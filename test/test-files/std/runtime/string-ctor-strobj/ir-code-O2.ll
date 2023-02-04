; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.1 = private unnamed_addr constant [8 x i8] c"S1: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [8 x i8] c"S2: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s1, ptr nonnull @anon.string.0) #2
  %.fca.0.load2 = load ptr, ptr %s1, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %1 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %__String__charptr_long_long %.fca.2.insert9)
  %.fca.0.load11 = load ptr, ptr %s1, align 8
  %.fca.0.insert12 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load11, 0
  %.fca.1.load14 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert15 = insertvalue %__String__charptr_long_long %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.load17 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert18 = insertvalue %__String__charptr_long_long %.fca.1.insert15, i64 %.fca.2.load17, 2
  call void @_mp__String__void__ctor__String(ptr nonnull %s2, %__String__charptr_long_long %.fca.2.insert18) #2
  %.fca.0.load20 = load ptr, ptr %s1, align 8
  %.fca.0.insert21 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load20, 0
  %.fca.1.load23 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert24 = insertvalue %__String__charptr_long_long %.fca.0.insert21, i64 %.fca.1.load23, 1
  %.fca.2.load26 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert27 = insertvalue %__String__charptr_long_long %.fca.1.insert24, i64 %.fca.2.load26, 2
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %__String__charptr_long_long %.fca.2.insert27)
  %.fca.0.load = load ptr, ptr %s2, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, %__String__charptr_long_long %.fca.2.insert)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_mp__String__void__ctor__String(ptr, %__String__charptr_long_long) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
