; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@anonymous.0 = constant [2 x i64] [i64 1, i64 2]
@l = constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %s1 = alloca i16, align 2
  %0 = alloca i16, align 2
  %shortArray = alloca i16, align 2
  %1 = alloca i16, align 2
  %shortArray1 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i1, align 1
  %4 = alloca [2 x i64], align 8
  %idx2 = alloca i32, align 4
  %l = alloca i64, align 8
  %l3 = alloca [2 x i64], align 8
  store i32 0, ptr %result, align 4
  store i16 5, ptr %s1, align 2
  %5 = load i16, ptr %s1, align 2
  store i16 -2, ptr %0, align 2
  %6 = load i16, ptr %0, align 2
  store i16 63, ptr %shortArray, align 2
  %7 = load i16, ptr %shortArray, align 2
  %8 = sub i16 0, %7
  store i16 %8, ptr %1, align 2
  %9 = load i16, ptr %1, align 2
  %10 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 0
  store i16 1, ptr %10, align 2
  %11 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 1
  store i16 %5, ptr %11, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 2
  store i16 %6, ptr %12, align 2
  %13 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 3
  store i16 %9, ptr %13, align 2
  %14 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 4
  store i16 0, ptr %14, align 2
  store i32 0, ptr %idx, align 4
  store i16 0, ptr %s, align 2
  %15 = load [5 x i16], ptr %shortArray1, align 2
  %16 = load i32, ptr %idx, align 4
  %17 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 %16
  %18 = load i16, ptr %17, align 2
  store i16 %18, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %19 = load i16, ptr %s, align 2
  %20 = sext i16 %19 to i32
  %21 = call i32 (ptr, ...) @printf(ptr @0, i32 %20)
  %22 = load i16, ptr %s, align 2
  %23 = and i16 %22, 1
  store i16 %23, ptr %2, align 2
  %24 = load i16, ptr %2, align 2
  %25 = sext i16 %24 to i32
  %26 = icmp eq i32 %25, 1
  store i1 %26, ptr %3, align 1
  %27 = load i1, ptr %3, align 1
  br i1 %27, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  store [2 x i64] [i64 1, i64 2], ptr %4, align 8
  store i32 0, ptr %idx2, align 4
  store i64 0, ptr %l, align 8
  store [2 x i64] [i64 1, i64 2], ptr %l3, align 8
  %28 = load [2 x i64], ptr %l3, align 8
  %29 = load i32, ptr %idx2, align 4
  %30 = getelementptr inbounds [2 x i64], ptr %l3, i32 0, i32 %29
  %31 = load i64, ptr %30, align 8
  store i64 %31, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %32 = load i64, ptr %l, align 8
  %33 = call i32 (ptr, ...) @printf(ptr @1, i64 %32)
  br label %foreach.end.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx4 = load i32, ptr %idx2, align 4
  %idx.inc = add i32 %idx4, 1
  store i32 %idx.inc, ptr %idx2, align 4
  %34 = getelementptr inbounds [2 x i64], ptr %l3, i32 0, i32 %idx.inc
  %35 = load i64, ptr %34, align 8
  store i64 %35, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %36 = load i32, ptr %idx2, align 4
  %37 = icmp ult i32 %36, 2
  br i1 %37, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6
  %idx5 = load i32, ptr %idx, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, ptr %idx, align 4
  %38 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 %idx.inc6
  %39 = load i16, ptr %38, align 2
  store i16 %39, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %40 = load i32, ptr %idx, align 4
  %41 = icmp ult i32 %40, 5
  br i1 %41, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4, %foreach.loop.l7
  %42 = call i32 (ptr, ...) @printf(ptr @2)
  %43 = load i32, ptr %result, align 4
  ret i32 %43
}

declare i32 @printf(ptr, ...)
