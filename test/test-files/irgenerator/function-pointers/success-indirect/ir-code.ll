; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [7 x i8] c"string\00", align 1
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define private i32 @_f__void__int__test__string(ptr %0) {
  %result = alloca i32, align 4
  %input = alloca ptr, align 8
  store ptr %0, ptr %input, align 8
  ret i32 12
}

define private i32 @"_f__void__int__invoke__f<int>(string)"(ptr %0) {
  %result = alloca i32, align 4
  %fctPtr = alloca ptr, align 8
  store ptr %0, ptr %fctPtr, align 8
  %2 = load ptr, ptr %fctPtr, align 8
  %3 = call i32 %2(ptr @anon.string.0)
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %testFct = alloca ptr, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store ptr @_f__void__int__test__string, ptr %testFct, align 8
  %1 = load ptr, ptr %testFct, align 8
  %2 = call i32 @"_f__void__int__invoke__f<int>(string)"(ptr %1)
  store i32 %2, ptr %i, align 4
  %3 = load i32, ptr %i, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }