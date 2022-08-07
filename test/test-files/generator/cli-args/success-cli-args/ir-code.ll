; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Argc: %d\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Argv no. 0: %s\0A\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Argv no. 1: %s\0A\00", align 1

define i32 @main(i32 %0, i8** %1) {
entry.l1:
  %argc = alloca i32, align 4
  %argv = alloca i8**, align 8
  %result = alloca i32, align 4
  %2 = alloca i1, align 1
  store i32 %0, i32* %argc, align 4
  store i8** %1, i8*** %argv, align 8
  store i32 0, i32* %result, align 4
  %3 = load i32, i32* %argc, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %3)
  %5 = load i8**, i8*** %argv, align 8
  %6 = getelementptr inbounds i8*, i8** %5, i32 0
  %7 = load i8*, i8** %6, align 8
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @1, i32 0, i32 0), i8* %7)
  %9 = load i32, i32* %argc, align 4
  %10 = icmp sgt i32 %9, 1
  store i1 %10, i1* %2, align 1
  %11 = load i1, i1* %2, align 1
  br i1 %11, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %entry.l1
  %12 = load i8**, i8*** %argv, align 8
  %13 = getelementptr inbounds i8*, i8** %12, i32 1
  %14 = load i8*, i8** %13, align 8
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %14)
  br label %if.end.l4

if.end.l4:                                        ; preds = %if.then.l4, %entry.l1
  %16 = load i32, i32* %result, align 4
  ret i32 %16
}

declare i32 @printf(i8*, ...)
