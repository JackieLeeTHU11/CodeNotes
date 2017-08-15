import requests

result = requests.post('http://api.bupt-search.com:81/v2/detection/detect',
                        data={'api_key': 'KUYVaAd06zoWfBFS2M9N6qfGKgVS1v7Ckx1cPnhQ',
                              'api_secret': '4uSTnMDZqXqurWa4JwLT6YsSti4cth6R0K0JcdsL'},
                        files={'img_file': open('gaoxs.jpg', 'rb')})

print "detection results:", result.text

