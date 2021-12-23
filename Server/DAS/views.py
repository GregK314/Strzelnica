from django.http import HttpResponse
from .models import Reading_db
from .models import Sensor_report_db
from .models import test_control_db
from django.shortcuts import get_object_or_404, render
from django.core import serializers
from django.db.models import Count, Max
from django.http import JsonResponse
import random
import time
from datetime import datetime
from django.forms.models import model_to_dict
import json


def feed_rand(request):
    start = time.time()
    if request.method == 'GET':
        testrun = int(request.GET['tr'])
        samples = int(request.GET['smp'])
        labels = [x['sensor_tick'] for x in Reading_db.objects.filter(
            test_run=testrun).order_by('sensor_tick').values('sensor_tick')]
        if len(labels) == 0:
            tick_base = 1
        else:
            tick_base = labels[-1]
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
        curr_dt = datetime.now()
        timestamp = int(round(curr_dt.timestamp()))
        result = Reading_db.objects.create(
            sensor_id=request.GET['sensor_id'],
            sensor_reading=request.GET['sensor_reading'],
            sensor_tick=request.GET['sensor_tick'],
            test_run=1)
    return HttpResponse(uptime2())


def get(request):
    start = time.time()
    if request.GET.get('test_run', 0) == 0:
        test_run = 'latest'
    else:
        test_run = request.GET['test_run']
    run_number = -1
    if test_run == 'latest':
        test_runs = [x['test_run'] for x in Reading_db.objects.order_by(
            'test_run').values('test_run').distinct()]
        run_number = test_runs[-1]
    else:
        run_number = int(test_run)

    labels = [x['sensor_tick'] for x in Reading_db.objects.filter(
        test_run=run_number).order_by('sensor_tick').values('sensor_tick').distinct()]
    m_label_dict = {}
    datasets = []
    for l in labels:
        m_label_dict[l] = None

    sensors = [x['sensor_id'] for x in Reading_db.objects.filter(
        test_run=run_number).order_by('sensor_id').values('sensor_id').distinct()]

    for sensor in sensors:
        reading = Reading_db.objects.filter(test_run=run_number).filter(
            sensor_id=sensor).order_by('sensor_tick')
        label_dict = m_label_dict.copy()
        for r in reading:
            label_dict[r.sensor_tick] = r.sensor_reading
        data = list(label_dict.values())
        datasets.append({'label': "sensor " + str(sensor), 'data': data})
    output = {'labels': labels, 'datasets': datasets, 'test_run': run_number}
    end = time.time()
    print('get served time taken: '+str(end - start))
    return JsonResponse(output)


def chart(request):
    return render(request, 'html/charts.html')


def uptime2():
    with open('/proc/uptime', 'r') as f:
        uptime_seconds = int(float(f.readline().split()[0])*1000)
        return uptime_seconds


def report_in(request):
    # http://0.0.0.0:3000/report_in/?s_id=1&s=0&ts=1&ax=0&ay=0&az=0&msg=test
    if request.method == 'GET':
        curr_dt = datetime.now()
        timestamp = int(round(curr_dt.timestamp()*1000))
        obj, created = Sensor_report_db.objects.update_or_create(
            sensor_id=int(request.GET['s_id']),
            defaults={'time_s': int(request.GET['ts']),
                      'status': int(request.GET['s']),
                      'anglex': float(request.GET['ax']),
                      'angley': float(request.GET['ay']),
                      'anglez': float(request.GET['az']),
                      'message': request.GET['msg']
                      },
        )
        test_status = test_control_db.objects.filter(
            ctrl_name="test_status")[0].ctrl_stat
    return HttpResponse(uptime2()+","+str(test_status))


def report_out(request):
    if request.method == 'GET':
        # 'sensor_id','time','anglex','angley','anglez'
        #'Select * from das_sensor_report_db order by time_s group by sensor_id'
        #data = Sensor_report_db.objects.values('sensor_id','time').values('sensor_id').annotate(dummy=Count('sensor_id')).order_by()
        data = Sensor_report_db.objects.raw(
            'Select * from (Select * from das_sensor_report_db order by time_s desc) as foo group by foo.sensor_id')
        rows = [model_to_dict(x) for x in data]
        json_object = json.dumps(rows, indent=4)
    return HttpResponse(json_object)
