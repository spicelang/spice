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
  %boolean = alloca i1, align 1
  %nestedInstance = alloca %_s__Nested__string_boolptr, align 8
  %0 = alloca %_s__Nested__string_boolptr, align 8
  %1 = alloca i1*, align 8
  %instance = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %2 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %3 = alloca i32*, align 8
  %4 = alloca %_s__Nested__string_boolptr*, align 8
  %instance1 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %input, align 4
  store i1 true, i1* %boolean, align 1
  %5 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %5, align 8
  %6 = load i1, i1* %boolean, align 1
  store i1* %boolean, i1** %1, align 8
  store i1* %boolean, i1** %1, align 8
  %7 = load i1*, i1** %1, align 8
  %8 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %0, i32 0, i32 1
  store i1* %7, i1** %8, align 8
  %9 = load %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %0, align 8
  store %_s__Nested__string_boolptr %9, %_s__Nested__string_boolptr* %nestedInstance, align 8
  %10 = load i32, i32* %input, align 4
  store i32* %input, i32** %3, align 8
  store i32* %input, i32** %3, align 8
  %11 = load i32*, i32** %3, align 8
  %12 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 0
  store i32* %11, i32** %12, align 8
  %13 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 1
  store double 4.634000e+01, double* %13, align 8
  %14 = load %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %nestedInstance, align 8
  store %_s__Nested__string_boolptr* %nestedInstance, %_s__Nested__string_boolptr** %4, align 8
  store %_s__Nested__string_boolptr* %nestedInstance, %_s__Nested__string_boolptr** %4, align 8
  %15 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %4, align 8
  %16 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 2
  store %_s__Nested__string_boolptr* %15, %_s__Nested__string_boolptr** %16, align 8
  %17 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, align 8
  store %_s__TestStruct__intptr_double_Nestedptr %17, %_s__TestStruct__intptr_double_Nestedptr* %instance, align 8
  %18 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance, align 8
  store %_s__TestStruct__intptr_double_Nestedptr %18, %_s__TestStruct__intptr_double_Nestedptr* %instance1, align 8
  %19 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance, align 8
  %20 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance, i32 0, i32 2
  %21 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %20, align 8
  %22 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %20, align 8
  %23 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %22, i32 0, i32 1
  %24 = load i1*, i1** %23, align 8
  %25 = load i1, i1* %24, align 1
  store i1 %25, i1* %24, align 1
  %26 = load i1, i1* %24, align 1
  %27 = zext i1 %26 to i32
  %28 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance1, align 8
  %29 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance1, i32 0, i32 1
  %30 = load double, double* %29, align 8
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %27, double %30)
  %32 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance1, align 8
  %33 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %instance1, i32 0, i32 2
  %34 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %33, align 8
  %35 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %33, align 8
  %36 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %35, i32 0, i32 0
  %37 = load i8*, i8** %36, align 8
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %37)
  %39 = load i32, i32* %result, align 4
  ret i32 %39
}

declare i32 @printf(i8*, ...)
