; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Input was false\00", align 1

define private void @_Z13executeActionb(i1 %0) {
  %input = alloca i1, align 1
  store i1 %0, ptr %input, align 1
  %2 = load i1, ptr %input, align 1
  br i1 %2, label %if.then.L2, label %if.exit.L2

if.then.L2:                                       ; preds = %1
  ret void

if.exit.L2:                                       ; preds = %1
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  call void @_Z13executeActionb(i1 false)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
