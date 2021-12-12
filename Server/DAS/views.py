from django.http import HttpResponse
from .models import Reading_db
from django.shortcuts import get_object_or_404, render
from django.core import serializers
from django.http import JsonResponse


def feed(request):
    if request.method == 'GET':
        result = Reading_db.objects.create(
            sensor_id=request.GET['sensor_id'],
            sensor_reading=request.GET['sensor_reading'],
            sensor_tick=request.GET['sensor_tick'])
    print(result)
    return HttpResponse("OK")


def get(request):
    labels = [x['sensor_tick'] for x in Reading_db.objects.order_by('sensor_tick').values('sensor_tick').distinct()]
    m_label_dict = {}
    datasets = []
    for l in labels:
        m_label_dict[l] = None

    sensors = [x['sensor_id'] for x in Reading_db.objects.order_by('sensor_id').values('sensor_id').distinct()]

    for sensor in sensors:
        reading = Reading_db.objects.filter(sensor_id=sensor).order_by('sensor_tick')
        label_dict = m_label_dict.copy()
        for r in reading:
            label_dict[r.sensor_tick] = r.sensor_reading
        data = list(label_dict.values())
        datasets.append({'label': "sensor " + str(sensor), 'data': data})
    output = {'labels': labels, 'datasets': datasets}
    return JsonResponse(output)


def chart(request):
    return render(request, 'html/charts.html')
