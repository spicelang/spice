; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Vec = type { i32, i1 }

@anon.struct.0 = private unnamed_addr constant %struct.Vec { i32 11, i1 false }

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %v = alloca %struct.Vec, align 8
  %v1 = alloca %struct.Vec, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %v, ptr @anon.struct.0, i64 8, i1 false)
  store %struct.Vec { i32 11, i1 false }, ptr %v, align 4
  call void @_ZN3Vec5printEv(ptr %v)
  store %struct.Vec zeroinitializer, ptr %v1, align 4
  call void @_ZN3Vec5printEv(ptr %v1)
  %1 = load i32, ptr %result, align 4
  ret i32 %1
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare void @_ZN3Vec5printEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
