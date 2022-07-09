; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [13 x i8] c"Loop run %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Inner loop run %d\0A\00", align 1
@2 = private unnamed_addr constant [5 x i8] c"End.\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %counter = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i1, align 1
  %subCounter = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 0, i32* %0, align 4
  br label %for.cond5

for:                                              ; preds = %for.cond5
  %6 = load i32, i32* %0, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i32 %6)
  %8 = load i32, i32* %0, align 4
  %9 = icmp sge i32 %8, 5
  store i1 %9, i1* %1, align 1
  %10 = load i1, i1* %1, align 1
  br i1 %10, label %if.then, label %if.end3

if.then:                                          ; preds = %for
  store i32 100, i32* %2, align 4
  br label %for.cond

for1:                                             ; preds = %for.cond
  %11 = load i32, i32* %2, align 4
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0), i32 %11)
  %13 = load i32, i32* %2, align 4
  %14 = icmp eq i32 %13, 11
  store i1 %14, i1* %3, align 1
  %15 = load i1, i1* %3, align 1
  br i1 %15, label %if.then2, label %if.end

if.then2:                                         ; preds = %for1
  br label %for.inc4

if.end:                                           ; preds = %for1
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %16 = load i32, i32* %2, align 4
  %17 = sub i32 %16, 1
  store i32 %17, i32* %2, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.then
  %18 = load i32, i32* %2, align 4
  %19 = icmp sge i32 %18, 10
  store i1 %19, i1* %4, align 1
  %20 = load i1, i1* %4, align 1
  br i1 %20, label %for1, label %for.end

for.end:                                          ; preds = %for.cond
  br label %if.end3

if.end3:                                          ; preds = %for.end, %for
  br label %for.inc4

for.inc4:                                         ; preds = %if.end3, %if.then2
  %21 = load i32, i32* %0, align 4
  %22 = add i32 %21, 2
  store i32 %22, i32* %0, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc4, %entry
  %23 = load i32, i32* %0, align 4
  %24 = icmp slt i32 %23, 10
  store i1 %24, i1* %5, align 1
  %25 = load i1, i1* %5, align 1
  br i1 %25, label %for, label %for.end6

for.end6:                                         ; preds = %for.cond5
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %27 = load i32, i32* %result, align 4
  ret i32 %27
}

declare i32 @printf(i8*, ...)
