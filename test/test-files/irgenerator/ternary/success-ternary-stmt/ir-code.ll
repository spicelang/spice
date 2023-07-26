; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i32 @_Z3getv() {
  %result = alloca i32, align 4
  ret i32 12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %condition = alloca i1, align 1
  %r = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i1 true, ptr %condition, align 1
  %1 = load i1, ptr %condition, align 1
  %2 = call i32 @_Z3getv()
  %3 = select i1 %1, i32 %2, i32 24
  store i32 %3, ptr %r, align 4
  %4 = load i32, ptr %r, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
