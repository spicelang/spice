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
  store i32 0, ptr %result, align 4
  %doubleVar = alloca double, align 8
  store double 0.000000e+00, ptr %doubleVar, align 8
  %0 = load double, ptr %doubleVar, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @0, double %0)
  %intVar = alloca i32, align 4
  store i32 0, ptr %intVar, align 4
  %2 = load i32, ptr %intVar, align 4
  %3 = call i32 (ptr, ...) @printf(ptr @1, i32 %2)
  %shortVar = alloca i16, align 2
  store i16 0, ptr %shortVar, align 2
  %4 = load i16, ptr %shortVar, align 2
  %5 = sext i16 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr @2, i32 %5)
  %longVar = alloca i64, align 8
  store i64 0, ptr %longVar, align 8
  %7 = load i64, ptr %longVar, align 8
  %8 = call i32 (ptr, ...) @printf(ptr @3, i64 %7)
  %stringVar = alloca ptr, align 8
  store ptr @4, ptr %stringVar, align 8
  %9 = load ptr, ptr %stringVar, align 8
  %10 = call i32 (ptr, ...) @printf(ptr @5, ptr %9)
  %byteVar = alloca i8, align 1
  store i8 0, ptr %byteVar, align 1
  %11 = load i8, ptr %byteVar, align 1
  %12 = sext i8 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr @6, i32 %12)
  %charVar = alloca i8, align 1
  store i8 0, ptr %charVar, align 1
  %14 = load i8, ptr %charVar, align 1
  %15 = sext i8 %14 to i32
  %16 = call i32 (ptr, ...) @printf(ptr @7, i32 %15)
  %boolVar = alloca i1, align 1
  store i1 false, ptr %boolVar, align 1
  %17 = load i1, ptr %boolVar, align 1
  %18 = zext i1 %17 to i32
  %19 = call i32 (ptr, ...) @printf(ptr @8, i32 %18)
  %20 = alloca %_s__NestedStruct__int_string, align 8
  %21 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %20, i32 0, i32 0
  store i32 0, ptr %21, align 4
  %22 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %20, i32 0, i32 1
  store ptr @9, ptr %22, align 8
  %structArrayVar = alloca [4 x %_s__NestedStruct__int_string], align 8
  store [4 x %_s__NestedStruct__int_string] [%_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }, %_s__NestedStruct__int_string { i32 0, ptr @9 }], ptr %structArrayVar, align 8
  %23 = getelementptr inbounds [4 x %_s__NestedStruct__int_string], ptr %structArrayVar, i32 0, i32 2
  %24 = getelementptr inbounds %_s__NestedStruct__int_string, ptr %23, i32 0, i32 1
  %25 = load ptr, ptr %24, align 8
  %26 = call i32 (ptr, ...) @printf(ptr @10, ptr %25)
  %27 = load i32, ptr %result, align 4
  ret i32 %27
}

declare i32 @printf(ptr, ...)
