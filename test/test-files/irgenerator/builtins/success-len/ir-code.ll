; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.array.0 = private unnamed_addr constant [4 x i32] [i32 1, i32 2, i32 3, i32 4]
@printf.str.0 = private unnamed_addr constant [18 x i8] c"Array length: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [4 x i32], align 4
  %testIntArray = alloca [4 x i32], align 4
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 16, i1 false)
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], ptr %testIntArray, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 4)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
