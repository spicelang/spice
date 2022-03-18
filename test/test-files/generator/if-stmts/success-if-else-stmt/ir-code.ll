; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca i1, align 1
  %6 = alloca double, align 8
  %7 = alloca double, align 8
  %8 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 1, i32* %0, align 4
  store i32 1, i32* %1, align 4
  %9 = load i32, i32* %0, align 4
  %10 = load i32, i32* %1, align 4
  %11 = icmp ne i32 %9, %10
  store i1 %11, i1* %2, align 1
  %12 = load i1, i1* %2, align 1
  br i1 %12, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0))
  br label %if.end6

if.else:                                          ; preds = %entry
  store double 2.000000e+00, double* %3, align 8
  store double 3.141500e+00, double* %4, align 8
  %14 = load double, double* %3, align 8
  %15 = load double, double* %4, align 8
  %16 = fcmp oeq double %14, %15
  store i1 %16, i1* %5, align 1
  %17 = load i1, i1* %5, align 1
  br i1 %17, label %if.then1, label %if.else2

if.then1:                                         ; preds = %if.else
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0))
  br label %if.end5

if.else2:                                         ; preds = %if.else
  store double 2.000000e+00, double* %6, align 8
  store double 2.718300e+00, double* %7, align 8
  %19 = load double, double* %6, align 8
  %20 = load double, double* %7, align 8
  %21 = fcmp oeq double %19, %20
  store i1 %21, i1* %8, align 1
  %22 = load i1, i1* %8, align 1
  br i1 %22, label %if.then3, label %if.else4

if.then3:                                         ; preds = %if.else2
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @2, i32 0, i32 0))
  br label %if.end

if.else4:                                         ; preds = %if.else2
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @3, i32 0, i32 0))
  br label %if.end

if.end:                                           ; preds = %if.else4, %if.then3
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then1
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %if.then
  %25 = load i32, i32* %result, align 4
  ret i32 %25
}

declare i32 @printf(i8*, ...)
