; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Nested__string_boolptr = type { ptr, ptr }
%_s__TestStruct__intptr_double_Nestedptr = type { ptr, double, ptr }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@2 = private unnamed_addr constant [12 x i8] c"Output: %s\0A\00", align 1

define i32 @main() {
entry.l12:
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %boolean = alloca i1, align 1
  %0 = alloca %_s__Nested__string_boolptr, align 8
  %1 = alloca ptr, align 8
  %2 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i32 12, ptr %input, align 4
  store i1 true, ptr %boolean, align 1
  %5 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %0, i32 0, i32 0
  store ptr @0, ptr %5, align 8
  store ptr %boolean, ptr %1, align 8
  %6 = load ptr, ptr %1, align 8
  %7 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %0, i32 0, i32 1
  store ptr %6, ptr %7, align 8
  store ptr %input, ptr %3, align 8
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 0
  store ptr %8, ptr %9, align 8
  %10 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 1
  store double 4.634000e+01, ptr %10, align 8
  store ptr %0, ptr %4, align 8
  %11 = load ptr, ptr %4, align 8
  %12 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 2
  store ptr %11, ptr %12, align 8
  %13 = load %_s__TestStruct__intptr_double_Nestedptr, ptr %2, align 8
  %14 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 2
  %15 = load ptr, ptr %14, align 8
  %16 = load ptr, ptr %14, align 8
  %17 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %16, i32 0, i32 1
  %18 = load ptr, ptr %17, align 8
  %19 = load i1, ptr %18, align 1
  store i1 %19, ptr %18, align 1
  %20 = load i1, ptr %18, align 1
  %21 = zext i1 %20 to i32
  %22 = load %_s__TestStruct__intptr_double_Nestedptr, ptr %2, align 8
  %23 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 1
  %24 = load double, ptr %23, align 8
  %25 = call i32 (ptr, ...) @printf(ptr @1, i32 %21, double %24)
  %26 = load %_s__TestStruct__intptr_double_Nestedptr, ptr %2, align 8
  %27 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %2, i32 0, i32 2
  %28 = load ptr, ptr %27, align 8
  %29 = load ptr, ptr %27, align 8
  %30 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %29, i32 0, i32 0
  %31 = load ptr, ptr %30, align 8
  %32 = call i32 (ptr, ...) @printf(ptr @2, ptr %31)
  %33 = load i32, ptr %result, align 4
  ret i32 %33
}

declare i32 @printf(ptr, ...)
