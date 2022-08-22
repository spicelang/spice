; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@str = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"Thread 1 finished\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"Thread 2 finished\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Thread 3 finished\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = alloca {}, align 8
  %t1 = alloca ptr, align 8
  %t3 = alloca ptr, align 8
  store ptr null, ptr %t3, align 8
  %1 = alloca ptr, align 8
  %2 = call i32 @pthread_create(ptr nonnull %1, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %3 = load ptr, ptr %1, align 8
  store ptr %3, ptr %t1, align 8
  %4 = alloca { ptr, ptr }, align 8
  store ptr %t1, ptr %4, align 8
  %5 = getelementptr inbounds { ptr, ptr }, ptr %4, i64 0, i32 1
  store ptr %t3, ptr %5, align 8
  %6 = alloca ptr, align 8
  %7 = call i32 @pthread_create(ptr nonnull %6, ptr null, ptr nonnull @_thread1, ptr nonnull %4)
  %8 = load ptr, ptr %6, align 8
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr nonnull %9, ptr null, ptr nonnull @_thread2, ptr nonnull %0)
  %11 = load ptr, ptr %9, align 8
  store ptr %11, ptr %t3, align 8
  %12 = load ptr, ptr %t1, align 8
  %13 = call i32 @pthread_join(ptr %12, ptr null)
  %14 = call i32 @pthread_join(ptr %8, ptr null)
  %15 = load ptr, ptr %t3, align 8
  %16 = call i32 @pthread_join(ptr %15, ptr null)
  %puts = call i32 @puts(ptr nonnull @str)
  ret i32 0
}

define internal noalias ptr @_thread0(ptr nocapture readnone %0) {
thread.entry.l8:
  %1 = tail call i32 @usleep(i32 300000)
  %puts = tail call i32 @puts(ptr nonnull @str.1)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

define internal noalias ptr @_thread1(ptr nocapture readonly %0) {
thread.entry.l13:
  %1 = load ptr, ptr %0, align 8
  %2 = getelementptr inbounds { ptr, ptr }, ptr %0, i64 0, i32 1
  %3 = load ptr, ptr %2, align 8
  %4 = load ptr, ptr %1, align 8
  %5 = tail call i32 @pthread_join(ptr %4, ptr null)
  %6 = load ptr, ptr %3, align 8
  %7 = tail call i32 @pthread_join(ptr %6, ptr null)
  %puts = tail call i32 @puts(ptr nonnull @str.2)
  ret ptr null
}

declare i32 @pthread_join(ptr, ptr) local_unnamed_addr

define internal noalias ptr @_thread2(ptr nocapture readnone %0) {
thread.entry.l18:
  %1 = tail call i32 @usleep(i32 200000)
  %puts = tail call i32 @puts(ptr nonnull @str.3)
  ret ptr null
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
