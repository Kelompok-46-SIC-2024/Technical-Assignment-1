from flask import Flask, request, jsonify

app = Flask(__name__)

data_records = []

@app.route('/sensor/data', methods=["POST", "GET"])
def handle_data():
    if request.method == 'POST':
        try:
            incoming_data = request.get_json()
            temperature = incoming_data["temperature"]
            humidity = incoming_data["humidity"]
            timestamp = incoming_data["timestamp"]

            data_records.append({
                "temperature": temperature,
                "humidity": humidity,
                "timestamp": timestamp
            })

            return jsonify({'message': 'Data successfully stored'}), 200

        except Exception as e:
            return jsonify({'message': 'Error processing request', 'error': str(e)}), 400
    else:
        return jsonify({'data': data_records}), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)