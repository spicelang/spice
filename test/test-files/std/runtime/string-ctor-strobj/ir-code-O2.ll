; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@2 = private unnamed_addr constant [8 x i8] c"S1: %s\0A\00", align 1
@3 = private unnamed_addr constant [8 x i8] c"S2: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
  %s1 = alloca %_s__String__charptr_long_long, align 8
  %s2 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s1, ptr nonnull @0)
  %0 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s1)
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @2, ptr %0)
  %.fca.0.load3 = load ptr, ptr %s1, align 8
  %.fca.0.insert4 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load3, 0
  %.fca.1.gep5 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.1.load6 = load i64, ptr %.fca.1.gep5, align 8
  %.fca.1.insert7 = insertvalue %_s__String__charptr_long_long %.fca.0.insert4, i64 %.fca.1.load6, 1
  %.fca.2.gep8 = getelementptr inbounds %_s__String__charptr_long_long, ptr %s1, i64 0, i32 2
  %.fca.2.load9 = load i64, ptr %.fca.2.gep8, align 8
  %.fca.2.insert10 = insertvalue %_s__String__charptr_long_long %.fca.1.insert7, i64 %.fca.2.load9, 2
  call void @_mp__String__void__ctor__String(ptr nonnull %s2, %_s__String__charptr_long_long %.fca.2.insert10)
  call void @_mp__String__void__append__string(ptr nonnull %s2, ptr nonnull @1)
  %2 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s1)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @2, ptr %2)
  %4 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s2)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull @3, ptr %4)
  call void @_mp__String__void__dtor(ptr nonnull %s1)
  call void @_mp__String__void__dtor(ptr nonnull %s2)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__String__charptr__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__void__ctor__String(ptr, %_s__String__charptr_long_long) local_unnamed_addr

declare void @_mp__String__void__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
