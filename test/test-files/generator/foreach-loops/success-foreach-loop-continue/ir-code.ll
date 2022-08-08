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
  %shortArray = alloca i16, align 2
  %0 = alloca i16, align 2
  %shortArray1 = alloca i16, align 2
  %1 = alloca i16, align 2
  %shortArray2 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca i1, align 1
  %4 = alloca [2 x i64], align 8
  %idx3 = alloca i32, align 4
  %l = alloca i64, align 8
  %l4 = alloca [2 x i64], align 8
  store i32 0, ptr %result, align 4
  store i16 5, ptr %s1, align 2
  %5 = load i16, ptr %s1, align 2
  store i16 27, ptr %shortArray, align 2
  %6 = load i16, ptr %shortArray, align 2
  %7 = sub i16 0, %6
  store i16 %7, ptr %0, align 2
  %8 = load i16, ptr %0, align 2
  store i16 63, ptr %shortArray1, align 2
  %9 = load i16, ptr %shortArray1, align 2
  %10 = sub i16 0, %9
  store i16 %10, ptr %1, align 2
  %11 = load i16, ptr %1, align 2
  %12 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 0
  store i16 1, ptr %12, align 2
  %13 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 1
  store i16 %5, ptr %13, align 2
  %14 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 2
  store i16 25, ptr %14, align 2
  %15 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 3
  store i16 %8, ptr %15, align 2
  %16 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 4
  store i16 %11, ptr %16, align 2
  store i32 0, ptr %idx, align 4
  store i16 0, ptr %s, align 2
  %17 = load [5 x i16], ptr %shortArray2, align 2
  %18 = load i32, ptr %idx, align 4
  %19 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 %18
  %20 = load i16, ptr %19, align 2
  store i16 %20, ptr %s, align 2
  br label %foreach.loop.l4

foreach.loop.l4:                                  ; preds = %foreach.cond.l4, %entry.l1
  %21 = load i16, ptr %s, align 2
  %22 = sext i16 %21 to i32
  %23 = call i32 (ptr, ...) @printf(ptr @0, i32 %22)
  %24 = load i16, ptr %s, align 2
  %25 = and i16 %24, 1
  store i16 %25, ptr %2, align 2
  %26 = load i16, ptr %2, align 2
  %27 = sext i16 %26 to i32
  %28 = icmp eq i32 %27, 1
  store i1 %28, ptr %3, align 1
  %29 = load i1, ptr %3, align 1
  br i1 %29, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %foreach.loop.l4
  store [2 x i64] [i64 1, i64 2], ptr %4, align 8
  store i32 0, ptr %idx3, align 4
  store i64 0, ptr %l, align 8
  store [2 x i64] [i64 1, i64 2], ptr %l4, align 8
  %30 = load [2 x i64], ptr %l4, align 8
  %31 = load i32, ptr %idx3, align 4
  %32 = getelementptr inbounds [2 x i64], ptr %l4, i32 0, i32 %31
  %33 = load i64, ptr %32, align 8
  store i64 %33, ptr %l, align 8
  br label %foreach.loop.l7

foreach.loop.l7:                                  ; preds = %foreach.cond.l7, %if.then.l6
  %34 = load i64, ptr %l, align 8
  %35 = call i32 (ptr, ...) @printf(ptr @1, i64 %34)
  br label %foreach.inc.l4

foreach.inc.l7:                                   ; No predecessors!
  %idx5 = load i32, ptr %idx3, align 4
  %idx.inc = add i32 %idx5, 1
  store i32 %idx.inc, ptr %idx3, align 4
  %36 = getelementptr inbounds [2 x i64], ptr %l4, i32 0, i32 %idx.inc
  %37 = load i64, ptr %36, align 8
  store i64 %37, ptr %l, align 8
  br label %foreach.cond.l7

foreach.cond.l7:                                  ; preds = %foreach.inc.l7
  %38 = load i32, ptr %idx3, align 4
  %39 = icmp ult i32 %38, 2
  br i1 %39, label %foreach.loop.l7, label %foreach.end.l7

foreach.end.l7:                                   ; preds = %foreach.cond.l7
  br label %if.end.l6

if.end.l6:                                        ; preds = %foreach.end.l7, %foreach.loop.l4
  br label %foreach.inc.l4

foreach.inc.l4:                                   ; preds = %if.end.l6, %foreach.loop.l7
  %idx6 = load i32, ptr %idx, align 4
  %idx.inc7 = add i32 %idx6, 1
  store i32 %idx.inc7, ptr %idx, align 4
  %40 = getelementptr inbounds [5 x i16], ptr %shortArray2, i32 0, i32 %idx.inc7
  %41 = load i16, ptr %40, align 2
  store i16 %41, ptr %s, align 2
  br label %foreach.cond.l4

foreach.cond.l4:                                  ; preds = %foreach.inc.l4
  %42 = load i32, ptr %idx, align 4
  %43 = icmp ult i32 %42, 5
  br i1 %43, label %foreach.loop.l4, label %foreach.end.l4

foreach.end.l4:                                   ; preds = %foreach.cond.l4
  %44 = call i32 (ptr, ...) @printf(ptr @2)
  %45 = load i32, ptr %result, align 4
  ret i32 %45
}

declare i32 @printf(ptr, ...)
