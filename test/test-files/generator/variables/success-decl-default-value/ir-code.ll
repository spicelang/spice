; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__NestedStruct__int_string = type { i32, ptr }

@0 = private unnamed_addr constant [18 x i8] c"Double value: %f\0A\00", align 1
@1 = private unnamed_addr constant [15 x i8] c"Int value: %d\0A\00", align 1
@2 = private unnamed_addr constant [17 x i8] c"Short value: %d\0A\00", align 1
@3 = private unnamed_addr constant [16 x i8] c"Long value: %d\0A\00", align 1
@4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@5 = private unnamed_addr constant [18 x i8] c"String value: %s\0A\00", align 1
@6 = private unnamed_addr constant [16 x i8] c"Byte value: %d\0A\00", align 1
@7 = private unnamed_addr constant [16 x i8] c"Char value: %c\0A\00", align 1
@8 = private unnamed_addr constant [16 x i8] c"Bool value: %d\0A\00", align 1
@9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@10 = private unnamed_addr constant [24 x i8] c"Struct array value: %s\0A\00", align 1

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %doubleVar = alloca double, align 8
  %intVar = alloca i32, align 4
  %shortVar = alloca i16, align 2
  %longVar = alloca i64, align 8
  %stringVar = alloca ptr, align 8
  %byteVar = alloca i8, align 1
  %charVar = alloca i8, align 1
  %boolVar = alloca i1, align 1
  %0 = alloca %_s__NestedStruct__int_string, align 8
  %structArrayVar = alloca [4 x %_s__NestedStruct__int_string], align 8
  store i32 0, ptr %result, align 4
  store double 0.000000e+00, ptr %doubleVar, align 8
  %1 = load double, ptr %doubleVar, align 8
  %2 = call i32 (ptr, ...) @printf(ptr @0, double %1)
  store i32 0, ptr %intVar, align 4
  %3 = load i32, ptr %intVar, align 4
  %4 = call i32 (ptr, ...) @printf(ptr @1, i32 %3)
  store i16 0, ptr %shortVar, align 2
  %5 = load i16, ptr %shortVar, align 2
  %6 = sext i16 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr @2, i32 %6)
  store i64 0, ptr %longVar, align 8
  %8 = load i64, ptr %longVar, align 8
  %9 = call i32 (ptr, ...) @printf(ptr @3, i64 %8)
  store ptr @4, ptr %stringVar, align 8
  %10 = load ptr, ptr %stringVar, align 8
  %11 = call i32 (ptr, ...) @printf(ptr @5, ptr %10)
  store i8 0, ptr %byteVar, align 1
  %12 = load i8, ptr %byteVar, align 1
  %13 = sext i8 %12 to i32
  %14 = call i32 (ptr, ...) @printf(ptr @6, i32 %13)
  store i8 0, ptr %charVar, align 1
  %15 = load i8, ptr %charVar, align 1
  %16 = sext i8 %15 to i32
  %17 = call i32 (ptr, ...) @printf(ptr @7, i32 %16)
  store i1 false, ptr %boolVar, align 1
  %18 = load i1, ptr %boolVar, align 1
  %19 = zext i1 %18 to i32
  %20 = call i32 (ptr, ...) @printf(ptr @8, i32 %19)
  %21 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %0, i32 0, i32 0
  store i32 0, ptr %21, align 4
  %22 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %0, i32 0, i32 1
  store ptr @9, ptr %22, align 8
  store [4 x %_s__NestedStruct__int_string] [%_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }], ptr %structArrayVar, align 8
  %23 = load [4 x %_s__NestedStruct__int_string], ptr %structArrayVar, align 8
  %24 = getelementptr inbounds [4 x %_s__NestedStruct__int_string], ptr %structArrayVar, i32 0, i32 2
  %25 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %24, i32 0, i32 1
  %26 = load ptr, ptr %25, align 8
  %27 = call i32 (ptr, ...) @printf(ptr @10, ptr %26)
  %28 = load i32, ptr %result, align 4
  ret i32 %28
}

declare i32 @printf(ptr, ...)
