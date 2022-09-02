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
  store i32 0, ptr %result, align 4
  %input = alloca i32, align 4
  store i32 12, ptr %input, align 4
  %boolean = alloca i1, align 1
  store i1 true, ptr %boolean, align 1
  %0 = alloca %_s__Nested__string_boolptr, align 8
  %1 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %0, i32 0, i32 0
  store ptr @0, ptr %1, align 8
  %2 = alloca ptr, align 8
  store ptr %boolean, ptr %2, align 8
  %3 = load ptr, ptr %2, align 8
  %4 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %0, i32 0, i32 1
  store ptr %3, ptr %4, align 8
  %5 = alloca %_s__TestStruct__intptr_double_Nestedptr, align 8
  %6 = alloca ptr, align 8
  store ptr %input, ptr %6, align 8
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 0
  store ptr %7, ptr %8, align 8
  %9 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 1
  store double 4.634000e+01, ptr %9, align 8
  %10 = alloca ptr, align 8
  store ptr %0, ptr %10, align 8
  %11 = load ptr, ptr %10, align 8
  %12 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 2
  store ptr %11, ptr %12, align 8
  %13 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 2
  %14 = load ptr, ptr %13, align 8
  %15 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %14, i32 0, i32 1
  %16 = load ptr, ptr %15, align 8
  %17 = load i1, ptr %16, align 1
  store i1 %17, ptr %16, align 1
  %18 = load i1, ptr %16, align 1
  %19 = zext i1 %18 to i32
  %20 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 1
  %21 = load double, ptr %20, align 8
  %22 = call i32 (ptr, ...) @printf(ptr @1, i32 %19, double %21)
  %23 = getelementptr inbounds %_s__TestStruct__intptr_double_Nestedptr, ptr %5, i32 0, i32 2
  %24 = load ptr, ptr %23, align 8
  %25 = getelementptr inbounds %_s__Nested__string_boolptr, ptr %24, i32 0, i32 0
  %26 = load ptr, ptr %25, align 8
  %27 = call i32 (ptr, ...) @printf(ptr @2, ptr %26)
  %28 = load i32, ptr %result, align 4
  ret i32 %28
}

declare i32 @printf(ptr, ...)
