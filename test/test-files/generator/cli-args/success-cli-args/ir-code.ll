; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [10 x i8] c"Argc: %d\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Argv no. 0: %s\0A\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Argv no. 1: %s\0A\00", align 1

define i32 @main(i32 %0, ptr %1) {
entry.l1:
  %argc = alloca i32, align 4
  store i32 %0, ptr %argc, align 4
  %argv = alloca ptr, align 8
  store ptr %1, ptr %argv, align 8
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %2 = load i32, ptr %argc, align 4
  %3 = call i32 (ptr, ...) @printf(ptr @0, i32 %2)
  %4 = load ptr, ptr %argv, align 8
  %5 = getelementptr inbounds ptr, ptr %4, i32 0
  %6 = load ptr, ptr %5, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @1, ptr %6)
  %8 = load i32, ptr %argc, align 4
  %9 = icmp sgt i32 %8, 1
  %10 = alloca i1, align 1
  store i1 %9, ptr %10, align 1
  %11 = load i1, ptr %10, align 1
  br i1 %11, label %if.then.l4, label %if.end.l4

if.then.l4:                                       ; preds = %entry.l1
  %12 = load ptr, ptr %argv, align 8
  %13 = getelementptr inbounds ptr, ptr %12, i32 1
  %14 = load ptr, ptr %13, align 8
  %15 = call i32 (ptr, ...) @printf(ptr @2, ptr %14)
  br label %if.end.l4

if.end.l4:                                        ; preds = %if.then.l4, %entry.l1
  %16 = load i32, ptr %result, align 4
  ret i32 %16
}

declare i32 @printf(ptr, ...)
