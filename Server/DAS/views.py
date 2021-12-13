from django.http import HttpResponse
from .models import Reading_db
from django.shortcuts import get_object_or_404, render
from django.core import serializers
from django.http import JsonResponse
import random
import time

def feed_rand(request):
    start = time.time()
    if request.method == 'GET':
        testrun = int(request.GET['tr']) 
        samples = int(request.GET['smp'])
        labels = [x['sensor_tick'] for x in Reading_db.objects.filter(test_run=testrun).order_by('sensor_tick').values('sensor_tick')]
        if len(labels)==0:
            tick_base=1
        else:
            tick_base=labels[-1]
        objs = []
        print(samples)
        for i in range(samples):
            #objs.append([random.randint(1, 4),random.random()*100,random.randint(tick_base, tick_base+samples),1])
            objs.append(Reading_db(
                sensor_id=random.randint(1, 4),
                sensor_reading=random.random()*100,
                sensor_tick=random.randint(tick_base, tick_base+samples),
                test_run=testrun))
        result = Reading_db.objects.bulk_create(objs)
    end = time.time()
    print('feed_rand served time taken: '+str(end - start))
    return HttpResponse(100)


def feed(request):
    if request.method == 'GET':
        result = Reading_db.objects.create(
            sensor_id=request.GET['sensor_id'],
            sensor_reading=request.GET['sensor_reading'],
            sensor_tick=request.GET['sensor_tick'],
            test_run=request.GET['test_run'])
    print(result)
    return HttpResponse("OK")


def get(request):
    start = time.time()
    if request.GET.get('test_run',0)==0:
        test_run = 'latest'
    else:
        test_run = request.GET['test_run']
    run_number = -1
    if test_run == 'latest':
        test_runs = [x['test_run'] for x in Reading_db.objects.order_by('test_run').values('test_run').distinct()]
        run_number = test_runs[-1]    
    else:
        run_number = int(test_run)
        
    labels = [x['sensor_tick'] for x in Reading_db.objects.filter(test_run=run_number).order_by('sensor_tick').values('sensor_tick').distinct()]
    m_label_dict = {}
    datasets = []
    for l in labels:
        m_label_dict[l] = None

    sensors = [x['sensor_id'] for x in Reading_db.objects.filter(test_run=run_number).order_by('sensor_id').values('sensor_id').distinct()]

    for sensor in sensors:
        reading = Reading_db.objects.filter(test_run=run_number).filter(sensor_id=sensor).order_by('sensor_tick')
        label_dict = m_label_dict.copy()
        for r in reading:
            label_dict[r.sensor_tick] = r.sensor_reading
        data = list(label_dict.values())
        datasets.append({'label': "sensor " + str(sensor), 'data': data})
    output = {'labels': labels, 'datasets': datasets, 'test_run':run_number}
    end = time.time()
    print('get served time taken: '+str(end - start))
    return JsonResponse(output)


def chart(request):
    return render(request, 'html/charts.html')
