; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__char__Queue__charptr_long_long_long_long_int = type { ptr, i64, i64, i64, i64, i32 }

@printf.str.0 = private unnamed_addr constant [24 x i8] c"Size: %d, Capacity: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %__char__Queue__charptr_long_long_long_long_int, align 8
  %q1 = alloca %__char__Queue__charptr_long_long_long_long_int, align 8
  call void @_mp__Queue_char__void__ctor(ptr nonnull %1) #2
  %.fca.0.load = load ptr, ptr %1, align 8
  %.fca.1.gep2 = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %1, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep2, align 8
  %.fca.2.gep3 = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %1, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep3, align 8
  %.fca.3.gep4 = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %1, i64 0, i32 3
  %.fca.3.load = load i64, ptr %.fca.3.gep4, align 8
  %.fca.4.gep5 = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %1, i64 0, i32 4
  %.fca.4.load = load i64, ptr %.fca.4.gep5, align 8
  %.fca.5.gep6 = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %1, i64 0, i32 5
  %.fca.5.load = load i32, ptr %.fca.5.gep6, align 8
  store ptr %.fca.0.load, ptr %q1, align 8
  %.fca.1.gep = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %q1, i64 0, i32 1
  store i64 %.fca.1.load, ptr %.fca.1.gep, align 8
  %.fca.2.gep = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %q1, i64 0, i32 2
  store i64 %.fca.2.load, ptr %.fca.2.gep, align 8
  %.fca.3.gep = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %q1, i64 0, i32 3
  store i64 %.fca.3.load, ptr %.fca.3.gep, align 8
  %.fca.4.gep = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %q1, i64 0, i32 4
  store i64 %.fca.4.load, ptr %.fca.4.gep, align 8
  %.fca.5.gep = getelementptr inbounds %__char__Queue__charptr_long_long_long_long_int, ptr %q1, i64 0, i32 5
  store i32 %.fca.5.load, ptr %.fca.5.gep, align 8
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 72) #2
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 101) #2
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 108) #2
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 108) #2
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 111) #2
  call void @_mp__Queue_char__void__push__char(ptr nonnull %q1, i8 33) #2
  %2 = call i64 @_mf__Queue_char__long__getSize(ptr nonnull %q1) #2
  %3 = call i64 @_mf__Queue_char__long__getCapacity(ptr nonnull %q1) #2
  %4 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %2, i64 %3)
  %5 = call i1 @_mf__Queue_char__bool__isEmpty(ptr nonnull %q1) #2
  br i1 %5, label %while.exit.L12, label %while.body.L12

while.body.L12:                                   ; preds = %0, %while.body.L12
  %6 = call i8 @_mf__Queue_char__char__pop(ptr nonnull %q1) #2
  %7 = sext i8 %6 to i32
  %putchar = call i32 @putchar(i32 %7)
  %8 = call i1 @_mf__Queue_char__bool__isEmpty(ptr nonnull %q1) #2
  br i1 %8, label %while.exit.L12, label %while.body.L12

while.exit.L12:                                   ; preds = %while.body.L12, %0
  ret i32 0
}

declare void @_mp__Queue_char__void__ctor(ptr) local_unnamed_addr

declare void @_mp__Queue_char__void__push__char(ptr, i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i64 @_mf__Queue_char__long__getSize(ptr) local_unnamed_addr

declare i64 @_mf__Queue_char__long__getCapacity(ptr) local_unnamed_addr

declare i1 @_mf__Queue_char__bool__isEmpty(ptr) local_unnamed_addr

declare i8 @_mf__Queue_char__char__pop(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
