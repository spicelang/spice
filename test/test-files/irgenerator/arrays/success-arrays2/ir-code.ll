; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.array.0 = private unnamed_addr constant [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512]
@printf.str.0 = private unnamed_addr constant [17 x i8] c"intArray[3]: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"intArray[7]: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"intArray[9]: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [10 x i32], align 4
  %intArray = alloca [10 x i32], align 4
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 40, i1 false)
  store [10 x i32] [i32 1, i32 2, i32 4, i32 8, i32 16, i32 32, i32 64, i32 128, i32 256, i32 512], ptr %intArray, align 4
  %2 = getelementptr inbounds [10 x i32], ptr %intArray, i32 0, i32 3
  %3 = load i32, ptr %2, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = getelementptr inbounds [10 x i32], ptr %intArray, i32 0, i32 7
  %6 = load i32, ptr %5, align 4
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %6)
  %8 = getelementptr inbounds [10 x i32], ptr %intArray, i32 0, i32 9
  %9 = load i32, ptr %8, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %9)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
