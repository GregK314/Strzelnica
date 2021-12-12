from django.urls import path

from . import views

urlpatterns = [
    # ex: /polls/
    path('feed/', views.feed, name='feed'),
    # ex: /polls/5/
    path('get/', views.get, name='get'),
    path('', views.chart, name='chart'),
    
]