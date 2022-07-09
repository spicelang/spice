; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Nested__string_boolptr = type { i8*, i1* }
%_s__TestStruct__intptr_double_Nestedptr = type { i32*, double, %_s__Nested__string_boolptr* }

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
  %nestedInstance = alloca %_s__Nested__string_boolptr, align 8
  %2 = alloca %_s__Nested__string_boolptr, align 8
  %3 = alloca i1*, align 8
  %instance = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %4 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %5 = alloca i32*, align 8
  %6 = alloca %_s__Nested__string_boolptr*, align 8
  %instance1 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %0, align 4
  store i1 true, i1* %1, align 1
  %7 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %2, i32 0, i32 0
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %7, align 8
  store i1* %1, i1** %3, align 8
  %8 = load i1*, i1** %3, align 8
  %9 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %2, i32 0, i32 1
  store i1* %8, i1** %9, align 8
  store i32* %0, i32** %5, align 8
  %10 = load i32*, i32** %5, align 8
  %11 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 0
  store i32* %10, i32** %11, align 8
  %12 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 1
  store double 4.634000e+01, double* %12, align 8
  store %_s__Nested__string_boolptr* %2, %_s__Nested__string_boolptr** %6, align 8
  %13 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %6, align 8
  %14 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 2
  store %_s__Nested__string_boolptr* %13, %_s__Nested__string_boolptr** %14, align 8
  %15 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, align 8
  %16 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 2
  %17 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %16, align 8
  %18 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %16, align 8
  %19 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %18, i32 0, i32 1
  %20 = load i1*, i1** %19, align 8
  %21 = load i1, i1* %20, align 1
  store i1 %21, i1* %20, align 1
  %22 = load i1, i1* %20, align 1
  %23 = zext i1 %22 to i32
  %24 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, align 8
  %25 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 1
  %26 = load double, double* %25, align 8
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %23, double %26)
  %28 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, align 8
  %29 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %4, i32 0, i32 2
  %30 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %29, align 8
  %31 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %29, align 8
  %32 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %31, i32 0, i32 0
  %33 = load i8*, i8** %32, align 8
  %34 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %33)
  %35 = load i32, i32* %result, align 4
  ret i32 %35
}

declare i32 @printf(i8*, ...)
