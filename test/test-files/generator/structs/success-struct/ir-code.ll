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
entry.l12:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %boolean = alloca i1, align 1
  %0 = alloca %_s__Nested__string_boolptr, align 8
  %1 = alloca i1*, align 8
  %2 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %3 = alloca i32*, align 8
  %4 = alloca %_s__Nested__string_boolptr*, align 8
  store i32 0, i32* %result, align 4
  store i32 12, i32* %input, align 4
  store i1 true, i1* %boolean, align 1
  %5 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %0, i32 0, i32 0
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %5, align 8
  store i1* %boolean, i1** %1, align 8
  %6 = load i1*, i1** %1, align 8
  %7 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %0, i32 0, i32 1
  store i1* %6, i1** %7, align 8
  store i32* %input, i32** %3, align 8
  %8 = load i32*, i32** %3, align 8
  %9 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 0
  store i32* %8, i32** %9, align 8
  %10 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 1
  store double 4.634000e+01, double* %10, align 8
  store %_s__Nested__string_boolptr* %0, %_s__Nested__string_boolptr** %4, align 8
  %11 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %4, align 8
  %12 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 2
  store %_s__Nested__string_boolptr* %11, %_s__Nested__string_boolptr** %12, align 8
  %13 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, align 8
  %14 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 2
  %15 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %14, align 8
  %16 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %14, align 8
  %17 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %16, i32 0, i32 1
  %18 = load i1*, i1** %17, align 8
  %19 = load i1, i1* %18, align 1
  store i1 %19, i1* %18, align 1
  %20 = load i1, i1* %18, align 1
  %21 = zext i1 %20 to i32
  %22 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, align 8
  %23 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 1
  %24 = load double, double* %23, align 8
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @1, i32 0, i32 0), i32 %21, double %24)
  %26 = load %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, align 8
  %27 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, %_s__TestStruct__intptr_double_Nestedptr* %2, i32 0, i32 2
  %28 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %27, align 8
  %29 = load %_s__Nested__string_boolptr*, %_s__Nested__string_boolptr** %27, align 8
  %30 = getelementptr inbounds %_s__Nested__string_boolptr, %_s__Nested__string_boolptr* %29, i32 0, i32 0
  %31 = load i8*, i8** %30, align 8
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0), i8* %31)
  %33 = load i32, i32* %result, align 4
  ret i32 %33
}

declare i32 @printf(i8*, ...)
