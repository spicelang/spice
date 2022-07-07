; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Short %d\0A\00", align 1
@l = constant [2 x i64] [i64 1, i64 2]
@1 = private unnamed_addr constant [9 x i8] c"Long %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %s1 = alloca i16, align 2
  %shortArray = alloca [5 x i16], align 2
  %0 = alloca i16, align 2
  %1 = alloca i16, align 2
  %2 = alloca i16, align 2
  %3 = alloca [5 x i16], align 2
  %idx = alloca i32, align 4
  %s = alloca i16, align 2
  %4 = alloca i16, align 2
  %5 = alloca i1, align 1
  %idx1 = alloca i32, align 4
  %l = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  store i32 0, i32* %result, align 4
  store i16 5, i16* %s1, align 2
  store i16 1, i16* %0, align 2
  store i16 -2, i16* %1, align 2
  store i16 63, i16* %2, align 2
  %8 = load i16, i16* %2, align 2
  %9 = sub i16 0, %8
  store i16 %9, i16* %2, align 2
  %10 = load i16, i16* %0, align 2
  %11 = getelementptr inbounds [5 x i16], [5 x i16]* %3, i32 0, i32 0
  store i16 %10, i16* %11, align 2
  %12 = load i16, i16* %s1, align 2
  %13 = getelementptr inbounds [5 x i16], [5 x i16]* %3, i32 0, i32 1
  store i16 %12, i16* %13, align 2
  %14 = load i16, i16* %1, align 2
  %15 = getelementptr inbounds [5 x i16], [5 x i16]* %3, i32 0, i32 2
  store i16 %14, i16* %15, align 2
  %16 = load i16, i16* %2, align 2
  %17 = getelementptr inbounds [5 x i16], [5 x i16]* %3, i32 0, i32 3
  store i16 %16, i16* %17, align 2
  %18 = getelementptr inbounds [5 x i16], [5 x i16]* %3, i32 0, i32 4
  store i16 0, i16* %18, align 2
  %19 = load [5 x i16], [5 x i16]* %3, align 2
  store [5 x i16] %19, [5 x i16]* %shortArray, align 2
  store i32 0, i32* %idx, align 4
  store i16 0, i16* %s, align 2
  %20 = load [5 x i16], [5 x i16]* %shortArray, align 2
  %21 = load i32, i32* %idx, align 4
  %22 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %21
  %23 = load i16, i16* %22, align 2
  store i16 %23, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.cond7, %entry
  %24 = load i16, i16* %s, align 2
  %25 = sext i16 %24 to i32
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %25)
  %27 = load i16, i16* %s, align 2
  %28 = and i16 %27, 1
  store i16 %28, i16* %4, align 2
  %29 = load i16, i16* %4, align 2
  %30 = sext i16 %29 to i32
  %31 = icmp eq i32 %30, 1
  store i1 %31, i1* %5, align 1
  %32 = load i1, i1* %5, align 1
  br i1 %32, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 0, i64* %l, align 8
  store i64 1, i64* %6, align 8
  store i64 2, i64* %7, align 8
  %33 = load [2 x i64], [2 x i64]* @l, align 8
  %34 = load i32, i32* %idx1, align 4
  %35 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %34
  %36 = load i64, i64* %35, align 8
  store i64 %36, i64* %l, align 8
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.cond, %if.then
  %37 = load i64, i64* %l, align 8
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i64 %37)
  br label %foreach.end8

foreach.inc:                                      ; No predecessors!
  %idx3 = load i32, i32* %idx1, align 4
  %idx.inc = add i32 %idx3, 1
  store i32 %idx.inc, i32* %idx1, align 4
  %39 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %idx.inc
  %40 = load i64, i64* %39, align 8
  store i64 %40, i64* %l, align 8
  br label %foreach.cond

foreach.cond:                                     ; preds = %foreach.inc
  %41 = load i32, i32* %idx1, align 4
  %42 = icmp ule i32 %41, 1
  br i1 %42, label %foreach.loop2, label %foreach.end

foreach.end:                                      ; preds = %foreach.cond
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  br label %foreach.inc4

foreach.inc4:                                     ; preds = %if.end
  %idx5 = load i32, i32* %idx, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, i32* %idx, align 4
  %43 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %idx.inc6
  %44 = load i16, i16* %43, align 2
  store i16 %44, i16* %s, align 2
  br label %foreach.cond7

foreach.cond7:                                    ; preds = %foreach.inc4
  %45 = load i32, i32* %idx, align 4
  %46 = icmp ule i32 %45, 4
  br i1 %46, label %foreach.loop, label %foreach.end8

foreach.end8:                                     ; preds = %foreach.cond7, %foreach.loop2
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %48 = load i32, i32* %result, align 4
  ret i32 %48
}

declare i32 @printf(i8*, ...)
