; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@anonymous.0 = global [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %s1 = alloca i16, align 2
  %0 = alloca [5 x i16], align 2
  %shortArray = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %1 = alloca i16, align 2
  %2 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  store i32 0, ptr %result, align 4
  store i16 5, ptr %s1, align 2
  %3 = load i16, ptr %s1, align 2
  %4 = getelementptr inbounds [5 x i16], ptr %0, i32 0, i32 0
  store i16 1, ptr %4, align 2
  %5 = getelementptr inbounds [5 x i16], ptr %0, i32 0, i32 1
  store i16 %3, ptr %5, align 2
  %6 = getelementptr inbounds [5 x i16], ptr %0, i32 0, i32 2
  store i16 25, ptr %6, align 2
  %7 = getelementptr inbounds [5 x i16], ptr %0, i32 0, i32 3
  store i16 -27, ptr %7, align 2
  %8 = getelementptr inbounds [5 x i16], ptr %0, i32 0, i32 4
  store i16 -63, ptr %8, align 2
  %9 = load [5 x i16], ptr %0, align 2
  store [5 x i16] %9, ptr %shortArray, align 2
  store i32 0, ptr %idx, align 4
  store i16 0, ptr %s, align 2
  %10 = load [5 x i16], ptr %shortArray, align 2
  %11 = load i32, ptr %idx, align 4
  %12 = getelementptr inbounds [5 x i16], ptr %shortArray, i32 0, i32 %11
  %13 = load i16, ptr %12, align 2
  store i16 %13, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %14 = load i16, ptr %s, align 2
  %15 = sext i16 %14 to i32
  %16 = call i32 (ptr, ...) @printf(ptr @0, i32 %15)
  %17 = load i16, ptr %s, align 2
  %18 = and i16 %17, 1
  store i16 %18, ptr %1, align 2
  %19 = load i16, ptr %1, align 2
  %20 = sext i16 %19 to i32
  %21 = icmp eq i32 %20, 1
  store i1 %21, ptr %2, align 1
  %22 = load i1, ptr %2, align 1
  br i1 %22, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  store i32 0, ptr %idx1, align 4
  store i64 0, ptr %l, align 8
  %23 = load [2 x i64], ptr @anonymous.0, align 8
  %24 = load i32, ptr %idx1, align 4
  %25 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %24
  %26 = load i64, ptr %25, align 8
  store i64 %26, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %27 = load i64, ptr %l, align 8
  %28 = call i32 (ptr, ...) @printf(ptr @1, i64 %27)
  br label %foreach.end.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx2 = load i32, ptr %idx1, align 4
  %idx.inc = add i32 %idx2, 1
  store i32 %idx.inc, ptr %idx1, align 4
  %29 = getelementptr inbounds [2 x i64], ptr @anonymous.0, i32 0, i32 %idx.inc
  %30 = load i64, ptr %29, align 8
  store i64 %30, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %31 = load i32, ptr %idx1, align 4
  %32 = icmp ult i32 %31, 2
  br i1 %32, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6
  %idx3 = load i32, ptr %idx, align 4
  %idx.inc4 = add i32 %idx3, 1
  store i32 %idx.inc4, ptr %idx, align 4
  %33 = getelementptr inbounds [5 x i16], ptr %shortArray, i32 0, i32 %idx.inc4
  %34 = load i16, ptr %33, align 2
  store i16 %34, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %35 = load i32, ptr %idx, align 4
  %36 = icmp ult i32 %35, 5
  br i1 %36, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4, %foreach.loop.l7
  %37 = call i32 (ptr, ...) @printf(ptr @2)
  %38 = load i32, ptr %result, align 4
  ret i32 %38
}

declare i32 @printf(ptr, ...)
