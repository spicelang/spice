; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.2 = private unnamed_addr constant [15 x i8] c"Hello World!?!\00", align 1
@printf.str.13 = private unnamed_addr constant [12 x i8] c"Equals: %d\0A\00", align 1
@anon.string.3 = private unnamed_addr constant [14 x i8] c"Hello World!!\00", align 1
@printf.str.17 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1
@printf.str.18 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [15 x i8] c"Capacity: %d\0A\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.20 = private unnamed_addr constant [11 x i8] c"Empty: %d\0A\00", align 1
@printf.str.21 = private unnamed_addr constant [10 x i8] c"Empty: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s = alloca %__String__charptr_long_long, align 8
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s, ptr nonnull @anon.string.0) #3
  %4 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %4)
  %6 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %6)
  %8 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %8)
  call void @_mp__String__void__append__string(ptr nonnull %s, ptr nonnull @anon.string.1) #3
  %10 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %10)
  %12 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %13 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %12)
  %14 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %15 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %14)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 63) #3
  %16 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %17 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %16)
  %18 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %19 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %18)
  %20 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %21 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %20)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 33) #3
  %22 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %23 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %22)
  %24 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %25 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %24)
  %26 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %27 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %26)
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.2) #3
  %.fca.0.load2 = load ptr, ptr %1, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %28 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %s, %__String__charptr_long_long %.fca.2.insert9) #3
  %29 = zext i1 %28 to i32
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %29)
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.3) #3
  %.fca.0.load = load ptr, ptr %2, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %31 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %s, %__String__charptr_long_long %.fca.2.insert) #3
  %32 = zext i1 %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %32)
  call void @_mp__String__void__clear(ptr nonnull %s) #3
  %34 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %35 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %34)
  %36 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %37 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %36)
  %38 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %39 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %38)
  call void @_mp__String__void__reserve__long(ptr nonnull %s, i64 100) #3
  %40 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s) #3
  %41 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, ptr %40)
  %42 = call i64 @_mf__String__long__getLength(ptr nonnull %s) #3
  %43 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.18, i64 %42)
  %44 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s) #3
  %45 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i64 %44)
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @anon.string.4) #3
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %s, ptr noundef nonnull align 8 dereferenceable(24) %3, i64 24, i1 false)
  %46 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  %47 = zext i1 %46 to i32
  %48 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.20, i32 %47)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 97) #3
  %49 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s) #3
  %50 = zext i1 %49 to i32
  %51 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.21, i32 %50)
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

declare i1 @_mf__String__bool__isEqual__String(ptr, %__String__charptr_long_long) local_unnamed_addr

declare void @_mp__String__void__clear(ptr) local_unnamed_addr

declare void @_mp__String__void__reserve__long(ptr, i64) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nounwind }
