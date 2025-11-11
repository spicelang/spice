; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { { ptr, ptr }, i64 }

@COUNTER = private global i32 0
@llvm.used = appending global [1 x ptr] [ptr @tsan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tsan.module_ctor, ptr null }]

; Function Attrs: sanitize_thread
define private void @_Z6workerv() #0 {
  %1 = call ptr @llvm.returnaddress(i32 0)
  call void @__tsan_func_entry(ptr %1)
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %0
  %2 = load i32, ptr %i, align 4
  %3 = icmp slt i32 %2, 1000000
  br i1 %3, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %4 = load i32, ptr @COUNTER, align 4
  %5 = add nsw i32 %4, 1
  call void @__tsan_write4(ptr @COUNTER)
  store i32 %5, ptr @COUNTER, align 4
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %6 = load i32, ptr %i, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr %i, align 4
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  call void @__tsan_func_exit()
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable
define dso_local noundef i32 @main() #1 {
  %1 = call ptr @llvm.returnaddress(i32 0)
  call void @__tsan_func_entry(ptr %1)
  %result = alloca i32, align 4
  %thread1 = alloca %struct.Thread, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %thread2 = alloca %struct.Thread, align 8
  %fat.ptr1 = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store ptr @_Z6workerv, ptr %fat.ptr, align 8
  %2 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %2, align 8
  %3 = load { ptr, ptr }, ptr %fat.ptr, align 8
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %thread1, { ptr, ptr } %3)
  store ptr @_Z6workerv, ptr %fat.ptr1, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 1
  store ptr poison, ptr %4, align 8
  %5 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(24) %thread2, { ptr, ptr } %5)
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %thread1)
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(24) %thread2)
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %thread1)
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(24) %thread2)
  %6 = load i32, ptr %result, align 4
  call void @__tsan_func_exit()
  ret i32 %6
}

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr })

declare void @_ZN6Thread3runEv(ptr)

declare void @_ZN6Thread4joinEv(ptr)

declare void @__tsan_init()

; Function Attrs: nounwind uwtable
define internal void @tsan.module_ctor() #2 {
  call void @__tsan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tsan_func_entry(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_func_exit() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_begin() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_end() #3

; Function Attrs: nounwind
declare void @__tsan_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write1(ptr) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic8_store(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_exchange(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_add(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_sub(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_and(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_nand(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_or(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_xor(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_compare_exchange_val(ptr, i8, i8, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write2(ptr) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic16_store(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_exchange(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_add(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_sub(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_and(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_nand(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_or(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_xor(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_compare_exchange_val(ptr, i16, i16, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write4(ptr) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic32_store(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_exchange(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_add(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_sub(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_and(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_nand(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_or(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_xor(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_compare_exchange_val(ptr, i32, i32, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write8(ptr) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic64_store(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_exchange(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_add(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_sub(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_and(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_nand(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_or(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_xor(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_compare_exchange_val(ptr, i64, i64, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write16(ptr) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic128_store(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_exchange(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_add(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_sub(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_and(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_nand(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_or(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_xor(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_compare_exchange_val(ptr, i128, i128, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_update(ptr, ptr) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_read(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_thread_fence(i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_signal_fence(i32) #3

; Function Attrs: nounwind
declare ptr @__tsan_memmove(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memcpy(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memset(ptr, i32, i64) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress(i32 immarg) #4

attributes #0 = { sanitize_thread }
attributes #1 = { mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable }
attributes #2 = { nounwind uwtable "frame-pointer"="all" }
attributes #3 = { nounwind }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 4, !"nosanitize_thread", i32 1}
!5 = !{!"spice version dev (https://github.com/spicelang/spice)"}
