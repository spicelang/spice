; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@1 = private unnamed_addr constant [6 x i8] c"Spice\00", align 1
@2 = private unnamed_addr constant [13 x i8] c"programmers!\00", align 1
@3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@welcomeMessage = global [5 x ptr] [ptr @0, ptr @1, ptr @2, ptr @3, ptr @3]
@4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@5 = private unnamed_addr constant [17 x i8] c"Word no. %d: %s\0A\00", align 1

define i32 @main() {
entry.l1:
  %result = alloca i32, align 4
  %welcomeMessage = alloca [5 x ptr], align 8
  %i = alloca i32, align 4
  %word = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %0 = load [5 x ptr], ptr @welcomeMessage, align 8
  store [5 x ptr] %0, ptr %welcomeMessage, align 8
  store i32 0, ptr %i, align 4
  store ptr @4, ptr %word, align 8
  %1 = load [5 x ptr], ptr %welcomeMessage, align 8
  %2 = load i32, ptr %i, align 4
  %3 = getelementptr inbounds [5 x ptr], ptr %welcomeMessage, i32 0, i32 %2
  %4 = load ptr, ptr %3, align 8
  store ptr %4, ptr %word, align 8
  br label %foreach.loop.l3

foreach.loop.l3:                                  ; preds = %foreach.cond.l3, %entry.l1
  %5 = load i32, ptr %i, align 4
  %6 = load ptr, ptr %word, align 8
  %7 = call i32 (ptr, ...) @printf(ptr @5, i32 %5, ptr %6)
  br label %foreach.inc.l3

foreach.inc.l3:                                   ; preds = %foreach.loop.l3
  %idx = load i32, ptr %i, align 4
  %idx.inc = add i32 %idx, 1
  store i32 %idx.inc, ptr %i, align 4
  %8 = getelementptr inbounds [5 x ptr], ptr %welcomeMessage, i32 0, i32 %idx.inc
  %9 = load ptr, ptr %8, align 8
  store ptr %9, ptr %word, align 8
  br label %foreach.cond.l3

foreach.cond.l3:                                  ; preds = %foreach.inc.l3
  %10 = load i32, ptr %i, align 4
  %11 = icmp ult i32 %10, 5
  br i1 %11, label %foreach.loop.l3, label %foreach.end.l3

foreach.end.l3:                                   ; preds = %foreach.cond.l3
  %12 = load i32, ptr %result, align 4
  ret i32 %12
}

declare i32 @printf(ptr, ...)