; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__NestedStruct__int_string = type { i32, i8* }
%_s__TestStruct__intptr_NestedStructptr = type { i32*, %_s__NestedStruct__int_string* }

@0 = private unnamed_addr constant [18 x i8] c"Double value: %f\0A\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Int value: %d\0A\00", align 1
@2 = private unnamed_addr constant [17 x i8] c"Short value: %d\0A\00", align 1
@3 = private unnamed_addr constant [16 x i8] c"Long value: %d\0A\00", align 1
@4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@5 = private unnamed_addr constant [18 x i8] c"String value: %s\0A\00", align 1
@6 = private unnamed_addr constant [16 x i8] c"Byte value: %d\0A\00", align 1
@7 = private unnamed_addr constant [16 x i8] c"Char value: %c\0A\00", align 1
@8 = private unnamed_addr constant [16 x i8] c"Bool value: %d\0A\00", align 1
@9 = private unnamed_addr constant [23 x i8] c"Int pointer value: %p\0A\00", align 1
@10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@11 = private unnamed_addr constant [24 x i8] c"Struct array value: %s\0A\00", align 1
@12 = private unnamed_addr constant [31 x i8] c"Nested struct field value: %p\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %doubleVar = alloca double, align 8
  %intVar = alloca i32, align 4
  %shortVar = alloca i16, align 2
  %longVar = alloca i64, align 8
  %stringVar = alloca i8*, align 8
  %byteVar = alloca i8, align 1
  %charVar = alloca i8, align 1
  %boolVar = alloca i1, align 1
  %intPtrVar = alloca i32*, align 8
  %structArrayVar = alloca [4 x %_s__NestedStruct__int_string], align 8
  %0 = alloca %_s__NestedStruct__int_string, align 8
  %structVar = alloca %_s__TestStruct__intptr_NestedStructptr, align 8
  %1 = alloca %_s__TestStruct__intptr_NestedStructptr, align 8
  store i32 0, i32* %result, align 4
  store double 0.000000e+00, double* %doubleVar, align 8
  %2 = load double, double* %doubleVar, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @0, i32 0, i32 0), double %2)
  store i32 0, i32* %intVar, align 4
  %4 = load i32, i32* %intVar, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @1, i32 0, i32 0), i32 %4)
  store i16 0, i16* %shortVar, align 2
  %6 = load i16, i16* %shortVar, align 2
  %7 = zext i16 %6 to i32
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @2, i32 0, i32 0), i32 %7)
  store i64 0, i64* %longVar, align 8
  %9 = load i64, i64* %longVar, align 8
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @3, i32 0, i32 0), i64 %9)
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @4, i32 0, i32 0), i8** %stringVar, align 8
  %11 = load i8*, i8** %stringVar, align 8
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @5, i32 0, i32 0), i8* %11)
  store i8 0, i8* %byteVar, align 1
  %13 = load i8, i8* %byteVar, align 1
  %14 = zext i8 %13 to i32
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @6, i32 0, i32 0), i32 %14)
  store i8 0, i8* %charVar, align 1
  %16 = load i8, i8* %charVar, align 1
  %17 = zext i8 %16 to i32
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @7, i32 0, i32 0), i32 %17)
  store i1 false, i1* %boolVar, align 1
  %19 = load i1, i1* %boolVar, align 1
  %20 = zext i1 %19 to i32
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @8, i32 0, i32 0), i32 %20)
  store i32* null, i32** %intPtrVar, align 8
  %22 = load i32*, i32** %intPtrVar, align 8
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @9, i32 0, i32 0), i32* %22)
  %24 = getelementptr inbounds %_s__NestedStruct__int_string, %_s__NestedStruct__int_string* %0, i32 0, i32 0
  store i32 0, i32* %24, align 4
  %25 = getelementptr inbounds %_s__NestedStruct__int_string, %_s__NestedStruct__int_string* %0, i32 0, i32 1
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @10, i32 0, i32 0), i8** %25, align 8
  store [4 x %_s__NestedStruct__int_string] [%_s__NestedStruct__int_string { i32 0, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @10, i32 0, i32 0) }, %_s__NestedStruct__int_string { i32 0, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @10, i32 0, i32 0) }, %_s__NestedStruct__int_string { i32 0, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @10, i32 0, i32 0) }, %_s__NestedStruct__int_string { i32 0, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @10, i32 0, i32 0) }], [4 x %_s__NestedStruct__int_string]* %structArrayVar, align 8
  %26 = load [4 x %_s__NestedStruct__int_string], [4 x %_s__NestedStruct__int_string]* %structArrayVar, align 8
  %27 = getelementptr inbounds [4 x %_s__NestedStruct__int_string], [4 x %_s__NestedStruct__int_string]* %structArrayVar, i32 0, i32 2
  %28 = load i8*, i8** %25, align 8
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @11, i32 0, i32 0), i8* %28)
  %30 = getelementptr inbounds %_s__TestStruct__intptr_NestedStructptr, %_s__TestStruct__intptr_NestedStructptr* %1, i32 0, i32 0
  store i32* null, i32** %30, align 8
  %31 = getelementptr inbounds %_s__TestStruct__intptr_NestedStructptr, %_s__TestStruct__intptr_NestedStructptr* %1, i32 0, i32 1
  store %_s__NestedStruct__int_string* null, %_s__NestedStruct__int_string** %31, align 8
  store %_s__TestStruct__intptr_NestedStructptr zeroinitializer, %_s__TestStruct__intptr_NestedStructptr* %structVar, align 8
  %32 = load %_s__TestStruct__intptr_NestedStructptr, %_s__TestStruct__intptr_NestedStructptr* %structVar, align 8
  %33 = getelementptr inbounds %_s__TestStruct__intptr_NestedStructptr, %_s__TestStruct__intptr_NestedStructptr* %structVar, i32 0, i32 1
  %34 = load %_s__NestedStruct__int_string*, %_s__NestedStruct__int_string** %33, align 8
  %35 = load %_s__NestedStruct__int_string*, %_s__NestedStruct__int_string** %31, align 8
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @12, i32 0, i32 0), %_s__NestedStruct__int_string* %35)
  %37 = load i32, i32* %result, align 4
  ret i32 %37
}

declare i32 @printf(i8*, ...)
