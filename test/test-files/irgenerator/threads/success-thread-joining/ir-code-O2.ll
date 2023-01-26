; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@str = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"Thread 1 finished\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"Thread 2 finished\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Thread 3 finished\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca {}, align 8
  %t1 = alloca ptr, align 8
  %t3 = alloca ptr, align 8
  %2 = alloca { ptr, ptr }, align 8
  store ptr null, ptr %t3, align 8
  %3 = alloca ptr, align 8
  %4 = call i32 @pthread_create(ptr nonnull %3, ptr null, ptr nonnull @_thread0, ptr nonnull %1) #2
  %5 = load ptr, ptr %3, align 8
  store ptr %5, ptr %t1, align 8
  store ptr %t3, ptr %2, align 8
  %6 = getelementptr inbounds { ptr, ptr }, ptr %2, i64 0, i32 1
  store ptr %t1, ptr %6, align 8
  %7 = alloca ptr, align 8
  %8 = call i32 @pthread_create(ptr nonnull %7, ptr null, ptr nonnull @_thread1, ptr nonnull %2) #2
  %9 = load ptr, ptr %7, align 8
  %10 = alloca ptr, align 8
  %11 = call i32 @pthread_create(ptr nonnull %10, ptr null, ptr nonnull @_thread2, ptr nonnull %1) #2
  %12 = load ptr, ptr %10, align 8
  store ptr %12, ptr %t3, align 8
  %13 = call i32 @pthread_join(ptr %9, ptr null) #2
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret i32 0
}

define private noalias ptr @_thread0(ptr nocapture readnone %0) {
  %2 = tail call i32 @usleep(i32 300000)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

define private noalias ptr @_thread1(ptr nocapture readonly %0) {
  %2 = load ptr, ptr %0, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i64 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load ptr, ptr %4, align 8
  %6 = tail call i32 @pthread_join(ptr %5, ptr null)
  %7 = load ptr, ptr %2, align 8
  %8 = tail call i32 @pthread_join(ptr %7, ptr null)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  ret ptr null
}

declare i32 @pthread_join(ptr, ptr) local_unnamed_addr

define private noalias ptr @_thread2(ptr nocapture readnone %0) {
  %2 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
