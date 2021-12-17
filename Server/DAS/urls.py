from django.urls import path

from . import views

urlpatterns = [
    # ex: /polls/
    path('feed/', views.feed, name='feed'),
    path('feed_rand/', views.feed_rand, name='feed_rand'),
    path('get/', views.get, name='get'),
    path('', views.chart, name='chart'),
    path('report_in/', views.report_in, name='report_in'),
    path('report_out/', views.report_out, name='report_out'),
]