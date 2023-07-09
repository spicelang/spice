; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Person = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"John\00", align 1
@anon.string.1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 1
@anon.struct.0 = private unnamed_addr constant %struct.Person { ptr @anon.string.0, ptr @anon.string.1, i32 46 }
@printf.str.0 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %john = alloca %struct.Person, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %john, ptr @anon.struct.0, i64 24, i1 false)
  store %struct.Person { ptr @anon.string.0, ptr @anon.string.1, i32 46 }, ptr %john, align 8
  %age_addr = getelementptr inbounds %struct.Person, ptr %john, i32 0, i32 2
  store i32 47, ptr %age_addr, align 4
  %age_addr1 = getelementptr inbounds %struct.Person, ptr %john, i32 0, i32 2
  %1 = load i32, ptr %age_addr1, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
