; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define private i32 @_Z4testv() {
  %result = alloca i32, align 4
  ret i32 12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %fat.ptr = alloca { ptr, ptr }, align 8
  %testFct = alloca { ptr, ptr }, align 8
  %i = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = getelementptr inbounds { ptr, ptr }, ptr %fat.ptr, i32 0, i32 0
  store ptr @_Z4testv, ptr %1, align 8
  %2 = load { ptr, ptr }, ptr %fat.ptr, align 8
  store { ptr, ptr } %2, ptr %testFct, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %testFct, i32 0, i32 0
  %fct = load ptr, ptr %3, align 8
  %4 = call i32 %fct()
  store i32 %4, ptr %i, align 4
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
