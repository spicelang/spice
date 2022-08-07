; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [16 x i8] c"Input was false\00", align 1

define internal void @_p__void__executeAction__bool(i1 %0) {
entry.l1:
  %input = alloca i1, align 1
  store i1 %0, i1* %input, align 1
  %1 = load i1, i1* %input, align 1
  br i1 %1, label %if.then.l2, label %if.end.l2

if.then.l2:                                       ; preds = %entry.l1
  ret void

if.end.l2:                                        ; preds = %entry.l1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @0, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry.l6:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i1 false, i1* %0, align 1
  %2 = load i1, i1* %0, align 1
  call void @_p__void__executeAction__bool(i1 %2)
  store i1 true, i1* %1, align 1
  %3 = load i32, i32* %result, align 4
  ret i32 %3
}
