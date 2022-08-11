; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@anonymous.0 = global [2 x i64] [i64 1, i64 2]
@l = global [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %s1 = alloca i16, align 2
  %shortArray = alloca i16, align 2
  %0 = alloca i16, align 2
  %shortArray1 = alloca i16, align 2
  %1 = alloca i16, align 2
  %shortArray2 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i1, align 1
  %idx3 = alloca i32, align 4
  %l = alloca i64, align 8
  store i32 0, ptr %result, align 4
  store i16 5, ptr %s1, align 2
  %4 = load i16, ptr %s1, align 2
  store i16 27, ptr %shortArray, align 2
  %5 = load i16, ptr %shortArray, align 2
  %6 = sub i16 0, %5
  store i16 %6, ptr %0, align 2
  %7 = load i16, ptr %0, align 2
  store i16 63, ptr %shortArray1, align 2
  %8 = load i16, ptr %shortArray1, align 2
  %9 = sub i16 0, %8
  store i16 %9, ptr %1, align 2
  %10 = load i16, ptr %1, align 2
  %11 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 0
  store i16 1, ptr %11, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 1
  store i16 %4, ptr %12, align 2
  %13 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 2
  store i16 25, ptr %13, align 2
  %14 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 3
  store i16 %7, ptr %14, align 2
  %15 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 4
  store i16 %10, ptr %15, align 2
  store i32 0, ptr %idx, align 4
  store i16 0, ptr %s, align 2
  %16 = load [5 x i16], ptr %shortArray2, align 2
  %17 = load i32, ptr %idx, align 4
  %18 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 %17
  %19 = load i16, ptr %18, align 2
  store i16 %19, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %20 = load i16, ptr %s, align 2
  %21 = sext i16 %20 to i32
  %22 = call i32 (ptr, ...) @printf(ptr @0, i32 %21)
  %23 = load i16, ptr %s, align 2
  %24 = and i16 %23, 1
  store i16 %24, ptr %2, align 2
  %25 = load i16, ptr %2, align 2
  %26 = sext i16 %25 to i32
  %27 = icmp eq i32 %26, 1
  store i1 %27, ptr %3, align 1
  %28 = load i1, ptr %3, align 1
  br i1 %28, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  store i32 0, ptr %idx3, align 4
  store i64 0, ptr %l, align 8
  %29 = load [2 x i64], ptr @l, align 8
  %30 = load i32, ptr %idx3, align 4
  %31 = getelementptr inbounds [2 x i64], ptr @l, i32 0, i32 %30
  %32 = load i64, ptr %31, align 8
  store i64 %32, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %33 = load i64, ptr %l, align 8
  %34 = call i32 (ptr, ...) @printf(ptr @1, i64 %33)
  br label %foreach.inc.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx4 = load i32, ptr %idx3, align 4
  %idx.inc = add i32 %idx4, 1
  store i32 %idx.inc, ptr %idx3, align 4
  %35 = getelementptr inbounds [2 x i64], ptr @l, i32 0, i32 %idx.inc
  %36 = load i64, ptr %35, align 8
  store i64 %36, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %37 = load i32, ptr %idx3, align 4
  %38 = icmp ult i32 %37, 2
  br i1 %38, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6, %foreach.loop.l7
  %idx5 = load i32, ptr %idx, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, ptr %idx, align 4
  %39 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 %idx.inc6
  %40 = load i16, ptr %39, align 2
  store i16 %40, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %41 = load i32, ptr %idx, align 4
  %42 = icmp ult i32 %41, 5
  br i1 %42, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4
  %43 = call i32 (ptr, ...) @printf(ptr @2)
  %44 = load i32, ptr %result, align 4
  ret i32 %44
}

declare i32 @printf(ptr, ...)
