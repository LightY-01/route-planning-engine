import pandas as pd
from pyrosm import OSM

osm = OSM("data/mumbai.osm.pbf")
nodes, edges = osm.get_network(nodes=True,network_type="driving")

# get unique node ids from osm
# osm_node_ids = pd.unique(nodes["id"]) -> produced 269498 nodes and 296620 dir edges
# -> since we are filtering for driving, nodes list contain many isolated nodes
osm_node_ids = pd.unique(edges[["u","v"]].values.flatten()) # -> produced 269498 nodes and 296620 dir edges
# Got same number of nodes and edges! Which means pyrosm handled isolated nodes!

# map osm node ids to 0-based indices for faster access
id_map = {node_id: i for i, node_id in enumerate(osm_node_ids)}

active_nodes = nodes[nodes["id"].isin(osm_node_ids)].copy()
active_nodes['node_id'] = active_nodes['id'].map(id_map)
nodes_out = active_nodes[['node_id','lat','lon']].sort_values("node_id")
nodes_out.to_csv("data/nodes.csv",index=False)

# Correctly handle directionality based on OSM's 'oneway' rules
edges['direction'] = 'both'

oneway_yes = edges['oneway'] == 'yes'
oneway_rev = edges['oneway'] == '-1'
oneway_missing = edges['oneway'].isna() | edges['oneway'].isin(['None', ''])

is_motorway = edges['highway'].isin(['motorway', 'motorway_link'])
is_roundabout = edges['junction'].isin(['roundabout'])

edges.loc[oneway_yes, 'direction'] = 'forward'
edges.loc[oneway_rev, 'direction'] = 'backward'
default_oneway = (is_motorway | is_roundabout) & oneway_missing
edges.loc[default_oneway, 'direction'] = 'forward'

# Forward edges (forward or bidirectional)
forward_edges = edges[edges['direction'].isin(['forward', 'both'])].copy()
forward_edges['source_id'] = forward_edges['u'].map(id_map)
forward_edges['target_id'] = forward_edges['v'].map(id_map)
forward_edges.rename(columns={'length': 'distance'}, inplace=True)
forward_edges = forward_edges[['source_id', 'target_id', 'distance']]

# Backward edges (backward or bidirectional)
backward_edges = edges[edges['direction'].isin(['backward', 'both'])].copy()
backward_edges['source_id'] = backward_edges['v'].map(id_map)
backward_edges['target_id'] = backward_edges['u'].map(id_map)
backward_edges.rename(columns={'length': 'distance'}, inplace=True)
backward_edges = backward_edges[['source_id', 'target_id', 'distance']]

# Combine and save
edges_out = pd.concat([forward_edges, backward_edges], ignore_index=True)
edges_out.to_csv("data/edges.csv", index=False)

print("done")
print(f"Total Nodes: {len(nodes_out)}")
print(f"Total Directed Edges: {len(edges_out)}")
