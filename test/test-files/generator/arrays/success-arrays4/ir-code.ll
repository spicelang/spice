; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"Spice\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"programmers!\00", align 1
@3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@welcomeMessage = constant [5 x i8*] [i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @2, i32 0, i32 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @3, i32 0, i32 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @3, i32 0, i32 0)]
@4 = private unnamed_addr constant [17 x i8] c"Word no. %d: %s\0A\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %welcomeMessage = alloca [5 x i8*], align 8
  %0 = alloca i8*, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i8*, align 8
  %i = alloca i32, align 4
  %word = alloca i8*, align 8
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @1, i32 0, i32 0), i8** %1, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @2, i32 0, i32 0), i8** %2, align 8
  %3 = load [5 x i8*], [5 x i8*]* @welcomeMessage, align 8
  store [5 x i8*] %3, [5 x i8*]* %welcomeMessage, align 8
  store i32 0, i32* %i, align 4
  %4 = load [5 x i8*], [5 x i8*]* %welcomeMessage, align 8
  %5 = load i32, i32* %i, align 4
  %6 = getelementptr inbounds [5 x i8*], [5 x i8*]* %welcomeMessage, i32 0, i32 %5
  %7 = load i8*, i8** %6, align 8
  store i8* %7, i8** %word, align 8
  br label %foreach.loop

foreach.loop:                                     ; preds = %foreach.inc, %entry
  %8 = load i32, i32* %i, align 4
  %9 = load i8*, i8** %word, align 8
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @4, i32 0, i32 0), i32 %8, i8* %9)
  %11 = load i32, i32* %i, align 4
  %12 = icmp slt i32 %11, 4
  br i1 %12, label %foreach.inc, label %foreach.end

foreach.inc:                                      ; preds = %foreach.loop
  %idx = load i32, i32* %i, align 4
  %idx.inc = add i32 %idx, 1
  store i32 %idx.inc, i32* %i, align 4
  %13 = getelementptr inbounds [5 x i8*], [5 x i8*]* %welcomeMessage, i32 0, i32 %idx.inc
  %14 = load i8*, i8** %13, align 8
  store i8* %14, i8** %word, align 8
  br label %foreach.loop

foreach.end:                                      ; preds = %foreach.loop
  %15 = load i32, i32* %result, align 4
  ret i32 %15
}

declare i32 @printf(i8*, ...)
