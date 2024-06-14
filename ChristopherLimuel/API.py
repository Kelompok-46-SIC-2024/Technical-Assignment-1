from flask import Flask, request, jsonify

app = Flask(__name__)

data_list = []

@app.route('/sensor/data', methods=["POST", "GET"])
def sensor_data():
    if request.method == 'POST':
        try:
            payload = request.get_json()
            temperature = payload["temperature"]
            humidity = payload["humidity"]
            timestamp = payload["timestamp"]

            data_list.append({
                "temperature": temperature,
                "humidity": humidity,
                "timestamp": timestamp
            })

            return jsonify({'message': 'Data stored'}), 200

        except Exception as e:
            return jsonify({'message': 'Error handling request', 'error': str(e)}), 400
    else:
        return jsonify({'data': data_list}), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)