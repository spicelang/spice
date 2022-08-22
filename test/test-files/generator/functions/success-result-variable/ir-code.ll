; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"The age is: %d\00", align 1

define internal i32 @_f__void__getAge() {
entry.l1:
  %result = alloca i32, align 4
  %i = alloca i1, align 1
  store i1 false, ptr %i, align 1
  br i1 true, label %if.then.l3, label %if.else.l3

if.then.l3:                                       ; preds = %entry.l1
  store i32 20, ptr %result, align 4
  %0 = load i32, ptr %result, align 4
  ret i32 %0

if.else.l3:                                       ; preds = %entry.l1
  store i1 false, ptr %i, align 1
  %1 = load i1, ptr %i, align 1
  br i1 %1, label %if.then.l6, label %if.end.l6

if.then.l6:                                       ; preds = %if.else.l3
  store i32 19, ptr %result, align 4
  br label %if.end.l6

if.end.l6:                                        ; preds = %if.then.l6, %if.else.l3
  br label %if.end.l3

if.end.l3:                                        ; preds = %if.end.l6
  %2 = alloca i32, align 4
  store i32 15, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  ret i32 %3
}

define i32 @main() {
entry.l12:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = call i32 @_f__void__getAge()
  %1 = alloca i32, align 4
  store i32 %0, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = call i32 (ptr, ...) @printf(ptr @0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare i32 @printf(ptr, ...)
