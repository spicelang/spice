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
  %9 = mul i16 %8, -1
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
  %18 = load [5 x i16], [5 x i16]* %3, align 2
  store [5 x i16] %18, [5 x i16]* %shortArray, align 2
  store i32 0, i32* %idx, align 4
  %19 = load [5 x i16], [5 x i16]* %shortArray, align 2
  %20 = load i32, i32* %idx, align 4
  %21 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %20
  %22 = load i16, i16* %21, align 2
  store i16 %22, i16* %s, align 2
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc4, %entry
  %23 = load i16, i16* %s, align 2
  %24 = zext i16 %23 to i32
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %24)
  %26 = load i16, i16* %s, align 2
  %27 = and i16 %26, 1
  store i16 %27, i16* %4, align 2
  %28 = load i16, i16* %4, align 2
  %29 = sext i16 %28 to i32
  %30 = icmp eq i32 %29, 1
  store i1 %30, i1* %5, align 1
  %31 = load i1, i1* %5, align 1
  br i1 %31, label %if.then, label %if.end

if.then:                                          ; preds = %foreach.loop
  store i32 0, i32* %idx1, align 4
  store i64 1, i64* %6, align 8
  store i64 2, i64* %7, align 8
  %32 = load [2 x i64], [2 x i64]* @l, align 8
  %33 = load i32, i32* %idx1, align 4
  %34 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %33
  %35 = load i64, i64* %34, align 8
  store i64 %35, i64* %l, align 8
  br label %foreach.loop2

foreach.loop2:                                    ; preds = %foreach.inc, %if.then
  %36 = load i64, i64* %l, align 8
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @1, i32 0, i32 0), i64 %36)
  br label %foreach.end7

foreach.inc:                                      ; No predecessors!
  %idx3 = load i32, i32* %idx1, align 4
  %idx.inc = add i32 %idx3, 1
  store i32 %idx.inc, i32* %idx1, align 4
  %38 = getelementptr inbounds [2 x i64], [2 x i64]* @l, i32 0, i32 %idx.inc
  %39 = load i64, i64* %38, align 8
  store i64 %39, i64* %l, align 8
  br label %foreach.loop2

foreach.end:                                      ; No predecessors!
  br label %if.end

if.end:                                           ; preds = %foreach.end, %foreach.loop
  %40 = load i32, i32* %idx, align 4
  %41 = icmp slt i32 %40, 4
  br i1 %41, label %foreach.inc4, label %foreach.end7

foreach.inc4:                                     ; preds = %if.end
  %idx5 = load i32, i32* %idx, align 4
  %idx.inc6 = add i32 %idx5, 1
  store i32 %idx.inc6, i32* %idx, align 4
  %42 = getelementptr inbounds [5 x i16], [5 x i16]* %shortArray, i32 0, i32 %idx.inc6
  %43 = load i16, i16* %42, align 2
  store i16 %43, i16* %s, align 2
  br label %foreach.loop

foreach.end7:                                     ; preds = %if.end, %foreach.loop2
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %45 = load i32, i32* %result, align 4
  ret i32 %45
}

declare i32 @printf(i8*, ...)
