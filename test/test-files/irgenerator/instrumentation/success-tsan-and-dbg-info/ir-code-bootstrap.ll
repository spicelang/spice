; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { %struct.Lambda, i64 }
%struct.Lambda = type { { ptr, ptr, i64 }, ptr, i64 }

@COUNTER = internal global i32 0, !dbg !0
@llvm.used = appending global [1 x ptr] [ptr @tsan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tsan.module_ctor, ptr null }]

; Function Attrs: noinline nounwind optnone sanitize_thread uwtable
define internal void @_Z6workerv() #0 !dbg !14 {
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !21
  call void @__tsan_func_entry(ptr %1), !dbg !21
  %i = alloca i32, align 4
    #dbg_declare(ptr %i, !19, !DIExpression(), !22)
  store i32 0, ptr %i, align 4, !dbg !22
  br label %for.head.L8, !dbg !22

for.head.L8:                                      ; preds = %for.tail.L8, %0
  %2 = load i32, ptr %i, align 4, !dbg !23
  %3 = icmp slt i32 %2, 1000000, !dbg !23
  br i1 %3, label %for.body.L8, label %for.exit.L8, !dbg !23

for.body.L8:                                      ; preds = %for.head.L8
  %4 = load i32, ptr @COUNTER, align 4, !dbg !24
  %5 = add nsw i32 %4, 1, !dbg !24
  call void @__tsan_write4(ptr @COUNTER), !dbg !24
  store i32 %5, ptr @COUNTER, align 4, !dbg !24
  br label %for.tail.L8, !dbg !25

for.tail.L8:                                      ; preds = %for.body.L8
  %6 = load i32, ptr %i, align 4, !dbg !26
  %7 = add nsw i32 %6, 1, !dbg !26
  store i32 %7, ptr %i, align 4, !dbg !26
  br label %for.head.L8, !dbg !26

for.exit.L8:                                      ; preds = %for.head.L8
  call void @__tsan_func_exit(), !dbg !27
  ret void, !dbg !27
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable
define noundef i32 @main() #1 !dbg !28 {
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !53
  call void @__tsan_func_entry(ptr %1), !dbg !53
  %thread1 = alloca %struct.Thread, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %thread2 = alloca %struct.Thread, align 8
  %fat.ptr1 = alloca { ptr, ptr, i64 }, align 8
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr, align 8, !dbg !54
  %2 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !54
  store ptr null, ptr %2, align 8, !dbg !54
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !54
  store i64 0, ptr %3, align 8, !dbg !54
  %4 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !54
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread1, { ptr, ptr, i64 } noundef %4), !dbg !54
    #dbg_declare(ptr %thread1, !32, !DIExpression(), !54)
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr1, align 8, !dbg !55
  %5 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 1, !dbg !55
  store ptr null, ptr %5, align 8, !dbg !55
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 2, !dbg !55
  store i64 0, ptr %6, align 8, !dbg !55
  %7 = load { ptr, ptr, i64 }, ptr %fat.ptr1, align 8, !dbg !55
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread2, { ptr, ptr, i64 } noundef %7), !dbg !55
    #dbg_declare(ptr %thread2, !52, !DIExpression(), !55)
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !56
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !57
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !58
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !59
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !60
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !60
  call void @__tsan_func_exit(), !dbg !60
  ret i32 0, !dbg !60
}

; Function Attrs: noinline nounwind optnone uwtable
define private void @_Z6workerv.fatthunk(ptr %0) #2 {
entry:
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !54
  call void @__tsan_func_entry(ptr %1), !dbg !54
  call void @_Z6workerv(), !dbg !54
  call void @__tsan_func_exit(), !dbg !54
  ret void, !dbg !54
}

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr, i64 })

declare void @_ZN6Thread3runEv(ptr)

declare void @_ZN6Thread4joinEv(ptr)

declare void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48))

declare void @__tsan_init()

; Function Attrs: nounwind uwtable
define internal void @tsan.module_ctor() #3 {
  call void @__tsan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tsan_func_entry(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_func_exit() #4

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_begin() #4

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_end() #4

; Function Attrs: nounwind
declare void @__tsan_read1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_write1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_write1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_read1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_write1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_read_write1(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write1(ptr) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_load(ptr, i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic8_store(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_exchange(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_add(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_sub(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_and(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_nand(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_or(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_xor(ptr, i8, i32) #4

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_compare_exchange_val(ptr, i8, i8, i32, i32) #4

; Function Attrs: nounwind
declare void @__tsan_read2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_write2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_write2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_read2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_write2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_read_write2(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write2(ptr) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_load(ptr, i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic16_store(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_exchange(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_add(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_sub(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_and(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_nand(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_or(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_xor(ptr, i16, i32) #4

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_compare_exchange_val(ptr, i16, i16, i32, i32) #4

; Function Attrs: nounwind
declare void @__tsan_read4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_write4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_write4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_read4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_write4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_read_write4(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write4(ptr) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_load(ptr, i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic32_store(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_exchange(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_add(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_sub(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_and(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_nand(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_or(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_xor(ptr, i32, i32) #4

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_compare_exchange_val(ptr, i32, i32, i32, i32) #4

; Function Attrs: nounwind
declare void @__tsan_read8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_write8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_write8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_read8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_write8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_read_write8(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write8(ptr) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_load(ptr, i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic64_store(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_exchange(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_add(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_sub(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_and(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_nand(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_or(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_xor(ptr, i64, i32) #4

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_compare_exchange_val(ptr, i64, i64, i32, i32) #4

; Function Attrs: nounwind
declare void @__tsan_read16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_write16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_write16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_read16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_volatile_write16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_read_write16(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write16(ptr) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_load(ptr, i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic128_store(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_exchange(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_add(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_sub(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_and(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_nand(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_or(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_xor(ptr, i128, i32) #4

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_compare_exchange_val(ptr, i128, i128, i32, i32) #4

; Function Attrs: nounwind
declare void @__tsan_vptr_update(ptr, ptr) #4

; Function Attrs: nounwind
declare void @__tsan_vptr_read(ptr) #4

; Function Attrs: nounwind
declare void @__tsan_atomic_thread_fence(i32) #4

; Function Attrs: nounwind
declare void @__tsan_atomic_signal_fence(i32) #4

; Function Attrs: nounwind
declare ptr @__tsan_memmove(ptr, ptr, i64) #4

; Function Attrs: nounwind
declare ptr @__tsan_memcpy(ptr, ptr, i64) #4

; Function Attrs: nounwind
declare ptr @__tsan_memset(ptr, i32, i64) #4

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress.p0(i32 immarg) #5

attributes #0 = { noinline nounwind optnone sanitize_thread uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nounwind uwtable }
attributes #4 = { nounwind }
attributes #5 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.module.flags = !{!6, !7, !8, !9, !10, !11, !12}
!llvm.ident = !{!13}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "COUNTER", linkageName: "COUNTER", scope: !2, file: !3, line: 5, type: !5, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info")
!4 = !{!0}
!5 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!6 = !{i32 8, !"PIC Level", i32 2}
!7 = !{i32 7, !"PIE Level", i32 2}
!8 = !{i32 7, !"uwtable", i32 2}
!9 = !{i32 7, !"frame-pointer", i32 0}
!10 = !{i32 7, !"Dwarf Version", i32 5}
!11 = !{i32 2, !"Debug Info Version", i32 3}
!12 = !{i32 4, !"nosanitize_thread", i32 1}
!13 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!14 = distinct !DISubprogram(name: "worker", linkageName: "_Z6workerv", scope: !3, file: !3, line: 7, type: !15, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !18)
!15 = !DISubroutineType(types: !16)
!16 = !{!17}
!17 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!18 = !{!19}
!19 = !DILocalVariable(name: "i", scope: !20, file: !3, line: 8, type: !5)
!20 = distinct !DILexicalBlock(scope: !14, file: !3, line: 8, column: 5)
!21 = !DILocation(line: 0, scope: !14)
!22 = !DILocation(line: 8, column: 17, scope: !20)
!23 = !DILocation(line: 8, column: 24, scope: !20)
!24 = !DILocation(line: 9, column: 9, scope: !20)
!25 = !DILocation(line: 10, column: 5, scope: !20)
!26 = !DILocation(line: 8, column: 33, scope: !20)
!27 = !DILocation(line: 11, column: 1, scope: !14)
!28 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !3, file: !3, line: 13, type: !29, scopeLine: 13, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !31)
!29 = !DISubroutineType(types: !30)
!30 = !{!5}
!31 = !{!32, !52}
!32 = !DILocalVariable(name: "thread1", scope: !28, file: !3, line: 14, type: !33)
!33 = !DICompositeType(tag: DW_TAG_structure_type, name: "Thread", scope: !3, file: !3, line: 23, size: 384, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !34, identifier: "struct.Thread")
!34 = !{!35, !50}
!35 = !DIDerivedType(tag: DW_TAG_member, name: "threadRoutine", scope: !33, file: !3, line: 24, baseType: !36, size: 320, align: 64)
!36 = !DICompositeType(tag: DW_TAG_structure_type, name: "Lambda<p()>", scope: !3, file: !3, line: 31, size: 320, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !37, identifier: "struct.Lambda")
!37 = !{!38, !46, !49}
!38 = !DIDerivedType(tag: DW_TAG_member, name: "native", scope: !36, file: !3, line: 32, baseType: !39, size: 192, align: 64)
!39 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !3, file: !3, size: 192, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !40, identifier: "_lambda")
!40 = !{!41, !43, !44}
!41 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !39, file: !3, baseType: !42, size: 64, align: 64)
!42 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64, align: 64, dwarfAddressSpace: 0)
!43 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !39, file: !3, baseType: !42, size: 64, align: 64, offset: 64)
!44 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !39, file: !3, baseType: !45, size: 64, align: 64, offset: 128)
!45 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!46 = !DIDerivedType(tag: DW_TAG_member, name: "ownedCaptures", scope: !36, file: !3, line: 33, baseType: !47, size: 64, offset: 192)
!47 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !48, size: 64, dwarfAddressSpace: 0)
!48 = !DIBasicType(name: "byte", size: 8, encoding: DW_ATE_unsigned)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !36, file: !3, line: 34, baseType: !45, size: 64, offset: 256)
!50 = !DIDerivedType(tag: DW_TAG_member, name: "threadId", scope: !33, file: !3, line: 25, baseType: !51, size: 64, offset: 320)
!51 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!52 = !DILocalVariable(name: "thread2", scope: !28, file: !3, line: 15, type: !33)
!53 = !DILocation(line: 0, scope: !28)
!54 = !DILocation(line: 14, column: 29, scope: !28)
!55 = !DILocation(line: 15, column: 29, scope: !28)
!56 = !DILocation(line: 16, column: 5, scope: !28)
!57 = !DILocation(line: 17, column: 5, scope: !28)
!58 = !DILocation(line: 18, column: 5, scope: !28)
!59 = !DILocation(line: 19, column: 5, scope: !28)
!60 = !DILocation(line: 20, column: 1, scope: !28)
