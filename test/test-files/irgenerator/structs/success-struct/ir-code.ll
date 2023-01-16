; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Nested__string_boolptr = type { ptr, ptr }
%__TestStruct__intptr_double_Nestedptr = type { ptr, double, ptr }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Output: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %boolean = alloca i1, align 1
  %nestedInstance = alloca %__Nested__string_boolptr, align 8
  %instance1 = alloca %__TestStruct__intptr_double_Nestedptr, align 8
  store i32 0, ptr %result, align 4
  store i32 12, ptr %input, align 4
  store i1 true, ptr %boolean, align 1
  %1 = getelementptr inbounds %__Nested__string_boolptr, ptr %nestedInstance, i32 0, i32 0
  store ptr @anon.string.0, ptr %1, align 8
  %2 = getelementptr inbounds %__Nested__string_boolptr, ptr %nestedInstance, i32 0, i32 1
  store ptr %boolean, ptr %2, align 8
  %3 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 0
  store ptr %input, ptr %3, align 8
  %4 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 1
  store double 4.634000e+01, ptr %4, align 8
  %5 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 2
  store ptr %nestedInstance, ptr %5, align 8
  %6 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 2
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr inbounds %__Nested__string_boolptr, ptr %7, i32 0, i32 1
  %9 = load ptr, ptr %8, align 8
  %10 = load i1, ptr %9, align 1
  %11 = zext i1 %10 to i32
  %12 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 1
  %13 = load double, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %11, double %13)
  %15 = getelementptr inbounds %__TestStruct__intptr_double_Nestedptr, ptr %instance1, i32 0, i32 2
  %16 = load ptr, ptr %15, align 8
  %17 = getelementptr inbounds %__Nested__string_boolptr, ptr %16, i32 0, i32 0
  %18 = load ptr, ptr %17, align 8
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %18)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
