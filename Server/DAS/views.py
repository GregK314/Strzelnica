from django.http import HttpResponse
from .models import Reading_db
from django.shortcuts import get_object_or_404, render
from django.core import serializers

def feed(request):
    if request.method == 'GET':
        result = Reading_db.objects.create(
            sensor_id= request.GET['sensor_id'],
            sensor_reading= request.GET['sensor_reading'],
            sensor_tick= request.GET['sensor_tick'])
    print(result)
    return HttpResponse("OK")

def get(request):
    labels = [x['sensor_tick'] for x in Reading_db.objects.order_by('sensor_tick').values('sensor_tick').distinct()]
    sensors = [x['sensor_id'] for x in Reading_db.objects.order_by('sensor_id').values('sensor_id').distinct()]
    for sensor in sensors:
        reading = 

    labels = [x['sensor_tick'] for x in Reading_db.objects.order_by('sensor_tick').values('sensor_tick').distinct()]
    response= serializers.serialize("json",Reading_db.objects.all())
    return HttpResponse(response)

def chart(request):
    return render(request,'html/charts.html')
