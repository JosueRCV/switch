from sanic import Sanic
from sanic.response import json
import requests
app = Sanic("Switch")

@app.route("/post", methods=["POST"])
async def echo(request):
    if request.json:
        x=requests.post("http://104.131.182.170:8529/_db/_system/practica/estado", json=request.json)
        print(x)
        return json({"Respuesta":"Exitoso"})
    return json({"R": "No fue un json"})

@app.route("/get",methods=["GET"])
async def test(request):
   url = requests.get("http://104.131.182.170:8529/_db/_system/practica/estado")
   text = url.text
   print(text)
   return json(text)

if __name__ == "__main__":
  app.run(host="104.131.182.170", port=8000)


