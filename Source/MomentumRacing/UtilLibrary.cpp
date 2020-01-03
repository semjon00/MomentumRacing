// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilLibrary.h"

float UUtilLibrary::SumArray(TArray<float> arr) {
    float res = 0.0f;
    for (float element : arr) {
        res += element;
    }
    return res;
}
