; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%"_s_Nested_string_bool*" = type { i8*, i1* }
%"_s_TestStruct_int*_double_struct(Nested)*" = type { i32*, double, %"_s_Nested_string_bool*"* }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Output: %s\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %0 = alloca i32, align 4
  %boolean = alloca i1, align 1
  %1 = alloca i1, align 1
  %nestedInstance = alloca %"_s_Nested_string_bool*", align 8
  %2 = alloca %"_s_Nested_string_bool*", align 8
  %3 = alloca i8*, align 8
  %4 = alloca i1*, align 8
  %instance = alloca %"_s_TestStruct_int*_double_struct(Nested)*", align 8
  %5 = alloca %"_s_TestStruct_int*_double_struct(Nested)*", align 8
  %6 = alloca i32*, align 8
  %7 = alloca double, align 8
  %8 = alloca %"_s_Nested_string_bool*"*, align 8
  %instance1 = alloca %"_s_TestStruct_int*_double_struct(Nested)*", align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %0, align 4
  %9 = load i32, i32* %0, align 4
  store i32 %9, i32* %input, align 4
  store i1 true, i1* %1, align 1
  %10 = load i1, i1* %1, align 1
  store i1 %10, i1* %boolean, align 1
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %3, align 8
  %11 = load i8*, i8** %3, align 8
  %12 = getelementptr inbounds %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %2, i32 0, i32 0
  store i8* %11, i8** %12, align 8
  %13 = load i1, i1* %boolean, align 1
  store i1* %boolean, i1** %4, align 8
  store i1* %boolean, i1** %4, align 8
  %14 = load i1*, i1** %4, align 8
  %15 = getelementptr inbounds %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %2, i32 0, i32 1
  store i1* %14, i1** %15, align 8
  %16 = load %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %2, align 8
  store %"_s_Nested_string_bool*" %16, %"_s_Nested_string_bool*"* %nestedInstance, align 8
  %17 = load i32, i32* %input, align 4
  store i32* %input, i32** %6, align 8
  store i32* %input, i32** %6, align 8
  %18 = load i32*, i32** %6, align 8
  %19 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %5, i32 0, i32 0
  store i32* %18, i32** %19, align 8
  store double 4.634000e+01, double* %7, align 8
  %20 = load double, double* %7, align 8
  %21 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %5, i32 0, i32 1
  store double %20, double* %21, align 8
  %22 = load %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %nestedInstance, align 8
  store %"_s_Nested_string_bool*"* %nestedInstance, %"_s_Nested_string_bool*"** %8, align 8
  store %"_s_Nested_string_bool*"* %nestedInstance, %"_s_Nested_string_bool*"** %8, align 8
  %23 = load %"_s_Nested_string_bool*"*, %"_s_Nested_string_bool*"** %8, align 8
  %24 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %5, i32 0, i32 2
  store %"_s_Nested_string_bool*"* %23, %"_s_Nested_string_bool*"** %24, align 8
  %25 = load %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %5, align 8
  store %"_s_TestStruct_int*_double_struct(Nested)*" %25, %"_s_TestStruct_int*_double_struct(Nested)*"* %instance, align 8
  %26 = load %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance, align 8
  store %"_s_TestStruct_int*_double_struct(Nested)*" %26, %"_s_TestStruct_int*_double_struct(Nested)*"* %instance1, align 8
  %27 = load %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance, align 8
  %28 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance, i32 0, i32 2
  %29 = load %"_s_Nested_string_bool*"*, %"_s_Nested_string_bool*"** %28, align 8
  %30 = load %"_s_Nested_string_bool*"*, %"_s_Nested_string_bool*"** %24, align 8
  %31 = getelementptr inbounds %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %29, i32 0, i32 1
  %32 = load i1*, i1** %31, align 8
  %33 = load i1, i1* %32, align 1
  store i1 %33, i1* %32, align 1
  %34 = load i1, i1* %32, align 1
  %35 = zext i1 %34 to i32
  %36 = load %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance1, align 8
  %37 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance1, i32 0, i32 1
  %38 = load double, double* %37, align 8
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %35, double %38)
  %40 = load %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance1, align 8
  %41 = getelementptr inbounds %"_s_TestStruct_int*_double_struct(Nested)*", %"_s_TestStruct_int*_double_struct(Nested)*"* %instance1, i32 0, i32 2
  %42 = load %"_s_Nested_string_bool*"*, %"_s_Nested_string_bool*"** %41, align 8
  %43 = load %"_s_Nested_string_bool*"*, %"_s_Nested_string_bool*"** %24, align 8
  %44 = getelementptr inbounds %"_s_Nested_string_bool*", %"_s_Nested_string_bool*"* %42, i32 0, i32 0
  %45 = load i8*, i8** %44, align 8
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %45)
  %47 = load i32, i32* %result, align 4
  ret i32 %47
}

declare i32 @printf(i8*, ...)
