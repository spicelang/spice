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
  %1 = alloca i32, align 4
  %2 = alloca i1, align 1
  %3 = alloca i32, align 4
  %4 = alloca i1, align 1
  %subCounter = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i1, align 1
  %8 = alloca i32, align 4
  %9 = alloca i1, align 1
  %10 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %0, align 4
  %11 = load i32, i32* %0, align 4
  store i32 %11, i32* %counter, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.post5, %entry
  store i32 10000, i32* %1, align 4
  %12 = load i32, i32* %counter, align 4
  %13 = load i32, i32* %1, align 4
  %14 = icmp slt i32 %12, %13
  store i1 %14, i1* %2, align 1
  %15 = load i1, i1* %2, align 1
  br i1 %15, label %for, label %for.end6

for:                                              ; preds = %for.cond
  %16 = load i32, i32* %counter, align 4
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i32 %16)
  store i32 9001, i32* %3, align 4
  %18 = load i32, i32* %counter, align 4
  %19 = load i32, i32* %3, align 4
  %20 = icmp sge i32 %18, %19
  store i1 %20, i1* %4, align 1
  %21 = load i1, i1* %4, align 1
  br i1 %21, label %if.then, label %if.end4

if.then:                                          ; preds = %for
  store i32 100, i32* %5, align 4
  %22 = load i32, i32* %5, align 4
  store i32 %22, i32* %subCounter, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.post, %if.then
  store i32 10, i32* %6, align 4
  %23 = load i32, i32* %subCounter, align 4
  %24 = load i32, i32* %6, align 4
  %25 = icmp sge i32 %23, %24
  store i1 %25, i1* %7, align 1
  %26 = load i1, i1* %7, align 1
  br i1 %26, label %for2, label %for.end

for2:                                             ; preds = %for.cond1
  %27 = load i32, i32* %subCounter, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0), i32 %27)
  store i32 11, i32* %8, align 4
  %29 = load i32, i32* %subCounter, align 4
  %30 = load i32, i32* %8, align 4
  %31 = icmp eq i32 %29, %30
  store i1 %31, i1* %9, align 1
  %32 = load i1, i1* %9, align 1
  br i1 %32, label %if.then3, label %if.end

if.then3:                                         ; preds = %for2
  br label %for.end6

if.end:                                           ; preds = %for2
  br label %for.post

for.post:                                         ; preds = %if.end
  %33 = load i32, i32* %subCounter, align 4
  %34 = sub i32 %33, 1
  store i32 %34, i32* %subCounter, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %if.end4

if.end4:                                          ; preds = %for.end, %for
  br label %for.post5

for.post5:                                        ; preds = %if.end4
  store i32 2, i32* %10, align 4
  %35 = load i32, i32* %10, align 4
  %36 = load i32, i32* %counter, align 4
  %37 = add i32 %36, %35
  store i32 %37, i32* %counter, align 4
  br label %for.cond

for.end6:                                         ; preds = %if.then3, %for.cond
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @2, i32 0, i32 0))
  %39 = load i32, i32* %result, align 4
  ret i32 %39
}

declare i32 @printf(i8*, ...)
