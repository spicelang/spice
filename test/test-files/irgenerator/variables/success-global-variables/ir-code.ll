; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@TEST1 = private global i32 10
@TEST2 = private global ptr @anon.string.0
@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 1
@TEST3 = private global double 5.830000e+00
@TEST4 = private global i1 false
@anon.string.1 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@printf.str.0 = private unnamed_addr constant [32 x i8] c"Variable values: %d, %s, %f, %u\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 11, ptr @TEST1, align 4
  store ptr @anon.string.1, ptr @TEST2, align 8
  store double 5.840000e+00, ptr @TEST3, align 8
  store i1 true, ptr @TEST4, align 1
  %1 = load i32, ptr @TEST1, align 4
  %2 = load ptr, ptr @TEST2, align 8
  %3 = load double, ptr @TEST3, align 8
  %4 = load i1, ptr @TEST4, align 1
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1, ptr %2, double %3, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
