import pandas as pd
from pyrosm import OSM

osm = OSM("mumbai.osm.pbf")
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
nodes_out.to_csv("nodes.csv",index=False)

edges_out = edges[['u', 'v', 'length']].copy()
edges_out['source_id'] = edges_out['u'].map(id_map)
edges_out['target_id'] = edges_out['v'].map(id_map)
edges_out.rename(columns={'length': 'distance'}, inplace=True)
edges_out = edges_out[['source_id', 'target_id', 'distance']]
edges_out.to_csv("edges.csv",index=False)

print("done")
print(f"Total Nodes: {len(nodes_out)}")
print(f"Total Directed Edges: {len(edges_out)}")
