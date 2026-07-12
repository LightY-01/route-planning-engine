import json
import subprocess
from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/find_path', methods=['POST'])
def find_path():
    data = request.json
    start_lat = data.get('start_lat')
    start_lon = data.get('start_lon')
    end_lat = data.get('end_lat')
    end_lon = data.get('end_lon')

    try:
        # Runs C++ executable
        result = subprocess.run(
            ['main.exe'], 
            input=f"{start_lat}\n{start_lon}\n{end_lat}\n{end_lon}\n",
            capture_output=True,
            text=True,
            check=True
        )
        
        path_coordinates = json.loads(result.stdout)

        # Check if path was found
        if len(path_coordinates) > 0:
            return jsonify({"status": "success", "path": path_coordinates})
        else:
            return jsonify({"status": "error", "message": "No path found"})

    except json.JSONDecodeError:
        return jsonify({"status": "error", "message": f"C++ output invalid JSON: {result.stdout}"})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})

if __name__ == '__main__':
    app.run(debug=True, port=29000)