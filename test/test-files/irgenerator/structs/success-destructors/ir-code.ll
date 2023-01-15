; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Vector__bool_string = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1
@anon.struct.0 = private unnamed_addr constant %__Vector__bool_string { i1 true, ptr @anon.string.0 }
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %vec = alloca %__Vector__bool_string, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %vec, ptr @anon.struct.0, i64 16, i1 false)
  %1 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 0
  %2 = load i1, ptr %1, align 1
  %3 = zext i1 %2 to i32
  %4 = getelementptr inbounds %__Vector__bool_string, ptr %vec, i32 0, i32 1
  %5 = load ptr, ptr %4, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3, ptr %5)
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

; Function Attrs: argmemonly nocallback nofree nounwind willreturn
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { argmemonly nocallback nofree nounwind willreturn }
