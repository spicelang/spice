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
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.0) #2
  %.fca.0.load45 = load ptr, ptr %1, align 8
  %.fca.0.insert46 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load45, 0
  %.fca.1.gep47 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load48 = load i64, ptr %.fca.1.gep47, align 8
  %.fca.1.insert49 = insertvalue %__String__charptr_long_long %.fca.0.insert46, i64 %.fca.1.load48, 1
  %.fca.2.gep50 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load51 = load i64, ptr %.fca.2.gep50, align 8
  %.fca.2.insert52 = insertvalue %__String__charptr_long_long %.fca.1.insert49, i64 %.fca.2.load51, 2
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %__String__charptr_long_long %.fca.2.insert52)
  call void @_mp__String__void__ctor__String(ptr nonnull %2, %__String__charptr_long_long %.fca.2.insert52) #2
  %.fca.0.load = load ptr, ptr %2, align 8
  %.fca.0.insert4 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert5 = insertvalue %__String__charptr_long_long %.fca.0.insert4, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert6 = insertvalue %__String__charptr_long_long %.fca.1.insert5, i64 %.fca.2.load, 2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, %__String__charptr_long_long %.fca.2.insert52)
  %5 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, %__String__charptr_long_long %.fca.2.insert6)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_mp__String__void__ctor__String(ptr, %__String__charptr_long_long) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
