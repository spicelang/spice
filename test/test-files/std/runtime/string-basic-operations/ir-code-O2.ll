; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@printf.str.15 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1
@printf.str.16 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@printf.str.17 = private unnamed_addr constant [15 x i8] c"Capacity: %d\0A\0A\00", align 1
@anon.string.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.18 = private unnamed_addr constant [11 x i8] c"Empty: %d\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [10 x i8] c"Empty: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %__String__charptr_long_long, align 8
  %s = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.0) #3
  %.fca.0.load = load ptr, ptr %1, align 8
  %.fca.1.gep2 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep2, align 8
  %.fca.2.gep3 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep3, align 8
  store ptr %.fca.0.load, ptr %s, align 8
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s, i64 0, i32 1
  store i64 %.fca.1.load, ptr %.fca.1.gep, align 8
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s, i64 0, i32 2
  store i64 %.fca.2.load, ptr %.fca.2.gep, align 8
  %3 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %3)
  %5 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %6 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %5)
  %7 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %7)
  call void @_mp__String__void__append__string(ptr nonnull %s, ptr nonnull @anon.string.1) #3
  %9 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %9)
  %11 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %11)
  %13 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %14 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %13)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 63) #3
  %15 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %16 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %15)
  %17 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %18 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %17)
  %19 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %20 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %19)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 33) #3
  %21 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %22 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %21)
  %23 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %24 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %23)
  %25 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %25)
  call void @_mp__String__void__clear(ptr nonnull %s) #3
  %27 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %28 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %27)
  %29 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %29)
  %31 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %32 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %31)
  call void @_mp__String__void__reserve__long(ptr nonnull %s, i64 100) #3
  %33 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %34 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.15, ptr %33)
  %35 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %36 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %35)
  %37 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %38 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i64 %37)
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.2) #3
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %s, ptr noundef nonnull align 8 dereferenceable(24) %2, i64 24, i1 false)
  %39 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  %40 = zext i1 %39 to i32
  %41 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i32 %40)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 97) #3
  %42 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  %43 = zext i1 %42 to i32
  %44 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %43)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare ptr @_mf__String__charptr__getRaw(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

declare void @_mp__String__void__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__void__clear(ptr) local_unnamed_addr

declare void @_mp__String__void__reserve__long(ptr, i64) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nounwind }
