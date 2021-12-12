from django.http import HttpResponse
from .models import Reading_db
from django.shortcuts import get_object_or_404, render

def feed(request):
    if request.method == 'POST':
        form = DinnerForm(request.POST)
    if form.is_valid():
        name = form.cleaned_data['name']
        text = form.cleaned_data['text']
        diner = Dinner.objects.create(
                    name = name,
                    text = text,)

    return HttpResponse(200)

#def get(request):
#    response = "You're looking at the results of question %s."
#    return HttpResponse(response % question_id)