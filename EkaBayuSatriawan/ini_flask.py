from flask import Flask, request, jsonify

app = Flask(__name__)

records = []

@app.route('/sensor/data', methods=["POST", "GET"])
def data_handler():
    if request.method == 'POST':
        try:
            content = request.get_json()
            temperature = content["temperature"]
            humidity = content["humidity"]
            timestamp = content["timestamp"]

            records.append({
                "temperature": temperature,
                "humidity": humidity,
                "timestamp": timestamp
            })

            return jsonify({'message': 'Data saved'}), 200

        except Exception as e:
            return jsonify({'message': 'Error occurred', 'error': str(e)}), 400
    else:
        return jsonify({'records': records}), 200

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
