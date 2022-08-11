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
  %0 = alloca i16, align 2
  %shortArray = alloca i16, align 2
  %1 = alloca i16, align 2
  %shortArray1 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i1, align 1
  %idx2 = alloca i32, align 4
  %l = alloca i64, align 8
  store i32 0, ptr %result, align 4
  store i16 5, ptr %s1, align 2
  %4 = load i16, ptr %s1, align 2
  store i16 -2, ptr %0, align 2
  %5 = load i16, ptr %0, align 2
  store i16 63, ptr %shortArray, align 2
  %6 = load i16, ptr %shortArray, align 2
  %7 = sub i16 0, %6
  store i16 %7, ptr %1, align 2
  %8 = load i16, ptr %1, align 2
  %9 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 0
  store i16 1, ptr %9, align 2
  %10 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 1
  store i16 %4, ptr %10, align 2
  %11 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 2
  store i16 %5, ptr %11, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 3
  store i16 %8, ptr %12, align 2
  %13 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 4
  store i16 0, ptr %13, align 2
  store i32 0, ptr %idx, align 4
  store i16 0, ptr %s, align 2
  %14 = load [5 x i16], ptr %shortArray1, align 2
  %15 = load i32, ptr %idx, align 4
  %16 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 %15
  %17 = load i16, ptr %16, align 2
  store i16 %17, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %18 = load i16, ptr %s, align 2
  %19 = sext i16 %18 to i32
  %20 = call i32 (ptr, ...) @printf(ptr @0, i32 %19)
  %21 = load i16, ptr %s, align 2
  %22 = and i16 %21, 1
  store i16 %22, ptr %2, align 2
  %23 = load i16, ptr %2, align 2
  %24 = sext i16 %23 to i32
  %25 = icmp eq i32 %24, 1
  store i1 %25, ptr %3, align 1
  %26 = load i1, ptr %3, align 1
  br i1 %26, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  store i32 0, ptr %idx2, align 4
  store i64 0, ptr %l, align 8
  %27 = load [2 x i64], ptr @l, align 8
  %28 = load i32, ptr %idx2, align 4
  %29 = getelementptr inbounds [2 x i64], ptr @l, i32 0, i32 %28
  %30 = load i64, ptr %29, align 8
  store i64 %30, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %31 = load i64, ptr %l, align 8
  %32 = call i32 (ptr, ...) @printf(ptr @1, i64 %31)
  br label %foreach.end.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx3 = load i32, ptr %idx2, align 4
  %idx.inc = add i32 %idx3, 1
  store i32 %idx.inc, ptr %idx2, align 4
  %33 = getelementptr inbounds [2 x i64], ptr @l, i32 0, i32 %idx.inc
  %34 = load i64, ptr %33, align 8
  store i64 %34, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %35 = load i32, ptr %idx2, align 4
  %36 = icmp ult i32 %35, 2
  br i1 %36, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6
  %idx4 = load i32, ptr %idx, align 4
  %idx.inc5 = add i32 %idx4, 1
  store i32 %idx.inc5, ptr %idx, align 4
  %37 = getelementptr inbounds [5 x i16], ptr %shortArray1, i32 0, i32 %idx.inc5
  %38 = load i16, ptr %37, align 2
  store i16 %38, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %39 = load i32, ptr %idx, align 4
  %40 = icmp ult i32 %39, 5
  br i1 %40, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4, %foreach.loop.l7
  %41 = call i32 (ptr, ...) @printf(ptr @2)
  %42 = load i32, ptr %result, align 4
  ret i32 %42
}

declare i32 @printf(ptr, ...)
