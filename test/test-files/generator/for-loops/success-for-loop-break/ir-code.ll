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
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %subCounter = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i32 0, i32* %counter, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post5, %entry
  %4 = load i32, i32* %counter, align 4
  %5 = icmp slt i32 %4, 10000
  store i1 %5, i1* %0, align 1
  %6 = load i1, i1* %0, align 1
  br i1 %6, label %for, label %for.end6

for:                                              ; preds = %for.cond
  %7 = load i32, i32* %counter, align 4
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i32 %7)
  %9 = load i32, i32* %counter, align 4
  %10 = icmp sge i32 %9, 9001
  store i1 %10, i1* %1, align 1
  %11 = load i1, i1* %1, align 1
  br i1 %11, label %if.then, label %if.end4

if.then:                                          ; preds = %for
  store i32 100, i32* %subCounter, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.post, %if.then
  %12 = load i32, i32* %subCounter, align 4
  %13 = icmp sge i32 %12, 10
  store i1 %13, i1* %2, align 1
  %14 = load i1, i1* %2, align 1
  br i1 %14, label %for2, label %for.end

for2:                                             ; preds = %for.cond1
  %15 = load i32, i32* %subCounter, align 4
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0), i32 %15)
  %17 = load i32, i32* %subCounter, align 4
  %18 = icmp eq i32 %17, 11
  store i1 %18, i1* %3, align 1
  %19 = load i1, i1* %3, align 1
  br i1 %19, label %if.then3, label %if.end

if.then3:                                         ; preds = %for2
  br label %for.end6

if.end:                                           ; preds = %for2
  br label %for.post

for.post:                                         ; preds = %if.end
  %20 = load i32, i32* %subCounter, align 4
  %21 = sub i32 %20, 1
  store i32 %21, i32* %subCounter, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %if.end4

if.end4:                                          ; preds = %for.end, %for
  br label %for.post5

for.post5:                                        ; preds = %if.end4
  %22 = load i32, i32* %counter, align 4
  %23 = add i32 %22, 2
  store i32 %23, i32* %counter, align 4
  br label %for.cond

for.end6:                                         ; preds = %if.then3, %for.cond
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %25 = load i32, i32* %result, align 4
  ret i32 %25
}

declare i32 @printf(i8*, ...)
